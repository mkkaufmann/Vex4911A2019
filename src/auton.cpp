#include "auton.hpp"
#include "constants.hpp"
#include "subsystems/arm.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter2.hpp"

using namespace lib7842;
using namespace lib7842::units;
using namespace okapi;

std::shared_ptr<Motor> topLeft = std::make_shared<Motor>(4);
std::shared_ptr<Motor> topRight = std::make_shared<Motor>(-5);
std::shared_ptr<Motor> bottomRight = std::make_shared<Motor>(-7);
std::shared_ptr<Motor> bottomLeft = std::make_shared<Motor>(6);

std::shared_ptr<Motor> stackerMotor1{
    std::make_shared<Motor>(Constants::STACKER_TREAD_1_MOTOR_PORT)};
std::shared_ptr<Motor> stackerMotor2{
    std::make_shared<Motor>(Constants::STACKER_TREAD_2_MOTOR_PORT)};

// This is the model of the robot chassis
// It keeps track of the motors, encoder wheels, max Velocity (in RPM), and max
// Voltage
std::shared_ptr<ThreeEncoderXDriveModel> model =
    std::make_shared<ThreeEncoderXDriveModel>(
        // motors
        topLeft,     // topLeft
        topRight,    // topRight
        bottomRight, // bottomRight
        bottomLeft,  // bottomLeft
        // sensors
        std::make_shared<ADIEncoder>(5, 6, true), // left
        std::make_shared<ADIEncoder>(1, 2, true), // right
        std::make_shared<ADIEncoder>(3, 4, true), // middle
        // limits
        200,  // max Velocity
        12000 // max Voltage
    );

ChassisScales odomChassisScales = ChassisScales(
    {2.792_in, // Encoder wheel diameter
     10_in,    // Drive base width, assumes encoder wheels are the same distance
               // from the center
     0.5_in},  // Middle wheel distance, which doesn't matter to the math
    360);      // Encoder ticks per rotstion

ChassisScales driveChassisScales = ChassisScales(
    {4_in,  // Encoder wheel diameter
     9_in,  // Drive base width, assumes encoder wheels are the same distance
            // from the center
     0_in}, // Middle wheel distance, which doesn't matter to the math
    imev5GreenTPR); // Encoder ticks per rotstion

// keeps track of position and orientation based on bot measurements
std::shared_ptr<CustomOdometry> odom{std::make_shared<CustomOdometry>(
    model, odomChassisScales, TimeUtilFactory().create())};

std::shared_ptr<OdomXController> odomController{
    std::make_shared<OdomXController>(
        model, odom,
        // Distance PID - To mm
        // Used for strafing and driving
        std::make_unique<IterativePosPIDController>(
            0.0178 * 1.5, 0.000001, 0.0009, 0,
            TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
        // Turn PID - To Degree
        // Used for turning
        std::make_unique<IterativePosPIDController>(
            0.03, 0.00, 0.0003, 0,
            TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
        // Angle PID - To Degree
        // Used for turning
        std::make_unique<IterativePosPIDController>(
            0.02, 0, 0.0, 0,
            TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
        TimeUtilFactory().create())};

PathFollower follower(model, odom, driveChassisScales, 1_ft,
                      TimeUtilFactory().create());

PursuitLimits fullSpeedLimits{0.2_mps,  1.1_mps2, 0.75_mps,
                              0.4_mps2, 0_mps,    40_mps};
PursuitLimits halfSpeedLimits{0.1_mps,      1.1_mps2 / 2, 0.75_mps / 2,
                              0.4_mps2 / 2, 0_mps,        40_mps / 2};

// Wraps the odomController calls for easier autonomous programming
// Takes a target position and orientation

void driveToPoint(Vector target, QAngle targetAngle, double turnPriority,
                  Settler &&settler) {
  odomController->strafeToPoint(
      target, OdomController::makeAngleCalculator(targetAngle), turnPriority,
      std::move(settler));
}

// Same as above, but faces a point during the motion instead of a set
// orientation
void driveToPoint(Vector target, Vector pointToFace, double turnPriority,
                  QLength settleDistance, QAngle settleAngle) {

  odomController->strafeToPoint(
      target, OdomController::makeAngleCalculator(pointToFace), turnPriority,
      Settler().distanceErr(settleDistance).angleErr(settleAngle));
}

// Sets the rollers to full speed outward
void rollerOuttake() {
  stackerMotor1->moveVoltage(12000);
  stackerMotor2->moveVoltage(12000);
}

// Sets the rollers to a specific speed
// accepts t values in [-1, 1].
// negative values intake, positive values outtake
void rollerOuttake(double t) {
  stackerMotor1->moveVoltage(12000 * t);
  stackerMotor2->moveVoltage(12000 * t);
}

// Sets the motors to full speed inward
void rollerIntake() {
  stackerMotor1->moveVoltage(-12000);
  stackerMotor2->moveVoltage(-12000);
}

// Stops the rollers
void rollerStop() {
  stackerMotor1->moveVoltage(0);
  stackerMotor2->moveVoltage(0);
}

void placeStack(int waitMs){
  rollerStop();
  setArmDown();
  setTilterUp();
  pros::delay(waitMs);
  setArmLowMiddle();
};

void deploy(){
  stackerMotor1->moveRelative(600, 12000);
  stackerMotor2->moveRelative(600, 12000);
  pros::delay(400);
  setTilterDown();
  setArmLowMiddle();
}

void oneCube() {
  topLeft->moveRelative(900 / (4.0 * 3.1415) * 8, 8000);
  topRight->moveRelative(900 / (4.0 * 3.1415) * 8, 8000);
  bottomLeft->moveRelative(900 / (4.0 * 3.1415) * 8, 8000);
  bottomRight->moveRelative(900 / (4.0 * 3.1415) * 8, 8000);
  pros::delay(3000);
  topLeft->moveRelative(900 / (4.0 * 3.1415) * -8, 8000);
  topRight->moveRelative(900 / (4.0 * 3.1415) * -8, 8000);
  bottomLeft->moveRelative(900 / (4.0 * 3.1415) * -8, 8000);
  bottomRight->moveRelative(900 / (4.0 * 3.1415) * -8, 8000);
}

void fiveCubeBluePP(){
  deploy();
  odom->setState(State({9.7_ft, 11.4_ft}, 180_deg));
  auto line = QuinticPath 
    ( 
        {
            odom -> getState (),
            { 9.7_ft, 7.4_ft, 180_deg }
        },

        0.2
    )
    .generate ( 50 ); 
  auto lineup = QuinticPath 
    ( 
        {
            { 9.7_ft, 7.4_ft, 180_deg },
            { 10_ft, 10_ft, 135_deg }
        },

        0.2
    )
    .generate ( 50 ); 
  rollerIntake();
  follower.followPath(PathGenerator::generate(line, fullSpeedLimits));
  follower.followPath(PathGenerator::generate(lineup, fullSpeedLimits), true);
  odomController->turnToAngle(45_deg, lib7842::OdomController::pointTurn, Settler().angleErr(3_deg));
  auto scoringPosition = QuinticPath 
    ( 
        {
            odom -> getState (),
            { 11_ft, 11_ft, 45_deg }
        },

        0.2
    )
    .generate ( 50 ); 
  auto backup = QuinticPath 
    ( 
        {
        {11_ft, 11_ft, 45_deg},
            { 10_ft, 10_ft, 45_deg }
        },

        0.2
    )
    .generate ( 50 ); 
  follower.followPath(PathGenerator::generate(scoringPosition, fullSpeedLimits));
  setTilterUp();
  pros::delay(2000);
  follower.followPath(PathGenerator::generate(backup, fullSpeedLimits), true);

}

void fiveCubeRed() {

  deploy();
  // full speed to line
  driveToPoint({0_in, 12_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(8000);
  driveToPoint({0_in, 15_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 20_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(5300);
  driveToPoint({0_in, 40_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // rest of line
  model->setMaxVoltage(4000);
  driveToPoint({0_in, 48_in}, 0_deg);

  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({15_in, 12_in}, 135_deg, 2);
  setTilterMiddle();
  driveToPoint({16_in, 9_in}, 135_deg, 1,
               Settler().distanceErr(2_in).angleErr(5_deg));
  setTilterUp();
  pros::delay(2000);
  rollerOuttake(0.5);
  driveToPoint({0_in, 21_in}, 135_deg, 2,
               Settler().distanceErr(1_in).angleErr(2_deg));
}

void fiveCubeBlue() {

  deploy();
  // full speed to line
  driveToPoint({0_in, 12_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 15_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(6000);
  driveToPoint({0_in, 20_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(5300);
  driveToPoint({0_in, 40_in}, 0_deg, 1,
               Settler().distanceErr(7_in).angleErr(5_deg));
  // rest of line
  model->setMaxVoltage(4000);
  driveToPoint({0_in, 46_in}, 0_deg, 1,
               Settler().distanceErr(5_in).angleErr(30_deg));
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({-12_in, 7_in}, -135_deg, 2);
  setTilterMiddle();
  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  driveToPoint({-14_in, 5_in}, -135_deg, 1,
               Settler().distanceErr(2_in).angleErr(5_deg));
  setTilterUp();
  pros::delay(2000);
  rollerOuttake(0.5);
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 21_in}, -135_deg, 2,
               Settler().distanceErr(1_in).angleErr(2_deg));
}

void sixCubeRed() {}
void sixCubeBlue() {}

void sevenCubeRed() {
  odom->setState({0_ft, 0_ft, 0_deg});
  driveToPoint({0_ft, 0.5_ft}, 0_deg, 1, Settler().distanceErr(4_in).maxTime(1000_ms, TimeUtilFactory().create()));
  deploy();
  odom->setState({7.7_ft, 12_ft-11_ft, 180_deg-180_deg});
  model->setMaxVoltage(8000);
  driveToPoint({7.7_ft, 12_ft-8.8_ft}, 180_deg-180_deg, 1, Settler().distanceErr(4_in).maxTime(500_ms, TimeUtilFactory().create()));
  rollerIntake();
  model->setMaxVoltage(6000);
  driveToPoint({7.7_ft, 12_ft-8.8_ft}, 180_deg-180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(3_s, TimeUtilFactory().create()));
  model->setMaxVoltage(12000);
  driveToPoint({9.8_ft, 12_ft-10.6_ft}, 180_deg-205_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  driveToPoint({9.8_ft, 12_ft-10.6_ft}, 180_deg-180_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(2_s, TimeUtilFactory().create()));
  model->setMaxVoltage(6000);
  driveToPoint({9.8_ft, 12_ft-7.3_ft}, 180_deg-180_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  model->setMaxVoltage(12000);
  driveToPoint({10.5_ft, 12_ft-10.5_ft}, 180_deg-5_deg, 2, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  driveToPoint({10.5_ft, 12_ft-10.5_ft}, 180_deg-45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(3_s, TimeUtilFactory().create()));
  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  model->setMaxVoltage(6000);
  driveToPoint({11.5_ft, 12_ft-11.5_ft}, 180_deg-45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(1.5_s, TimeUtilFactory().create()));
  placeStack(1500);
  rollerOuttake(0.3);
  driveToPoint({9.5_ft, 12_ft-9.5_ft}, 180_deg-45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
}

void sevenCubeBlue() {
  odom->setState({0_ft, 0_ft, 0_deg});
  driveToPoint({0_ft, 0.5_ft}, 0_deg, 1, Settler().distanceErr(4_in).maxTime(1000_ms, TimeUtilFactory().create()));
  deploy();
  odom->setState({7.7_ft, 11_ft, 180_deg});
  model->setMaxVoltage(8000);
  driveToPoint({7.7_ft, 8.8_ft}, 180_deg, 1, Settler().distanceErr(4_in).maxTime(500_ms, TimeUtilFactory().create()));
  rollerIntake();
  model->setMaxVoltage(6000);
  driveToPoint({7.7_ft, 8.8_ft}, 180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(3_s, TimeUtilFactory().create()));
  model->setMaxVoltage(12000);
  driveToPoint({9.8_ft, 10.6_ft}, 205_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  driveToPoint({9.8_ft, 10.6_ft}, 180_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(2_s, TimeUtilFactory().create()));
  model->setMaxVoltage(6000);
  driveToPoint({9.8_ft, 7.3_ft}, 180_deg, 1.5, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  model->setMaxVoltage(12000);
  driveToPoint({10.5_ft, 10.5_ft}, 5_deg, 2, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
  driveToPoint({10.5_ft, 10.5_ft}, 45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(3_s, TimeUtilFactory().create()));
  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  model->setMaxVoltage(6000);
  driveToPoint({11.5_ft, 11.5_ft}, 45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(1.5_s, TimeUtilFactory().create()));
  placeStack(1500);
  rollerOuttake(0.3);
  driveToPoint({9.5_ft, 9.5_ft}, 45_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(6_s, TimeUtilFactory().create()));
}


void nineCubeRed() {
  deploy();
  odom->setState({9.7_ft, 1_ft, 0_deg});
  model->setMaxVoltage(8000);
  driveToPoint({9.7_ft, 4.6_ft}, 0_deg, 1, Settler().distanceErr(4_in).maxTime(500_ms, TimeUtilFactory().create()));
  rollerIntake();
  driveToPoint({9.7_ft, 4.6_ft}, 0_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({9.0_ft, 5_ft}, -30_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({8.0_ft, 5_ft}, -135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({7.7_ft, 4.5_ft}, -180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({7.7_ft, 3_ft}, -180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({10.5_ft, 1.5_ft}, 135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({12_ft, 0_ft}, 135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(1_s, TimeUtilFactory().create()));
  placeStack(1500);
  rollerOuttake(0.3);
  driveToPoint({9.5_ft, 2.5_ft}, 135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
}
void nineCubeBlue() {
  deploy();
  odom->setState({9.7_ft, 12_ft-1_ft, 180_deg+0_deg});
  model->setMaxVoltage(8000);
  driveToPoint({9.7_ft, 12_ft-4.6_ft}, 180_deg+0_deg, 1, Settler().distanceErr(4_in).maxTime(500_ms, TimeUtilFactory().create()));
  rollerIntake();
  driveToPoint({9.7_ft, 12_ft-4.6_ft}, 180_deg+0_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({9.0_ft, 12_ft-5_ft}, 180_deg-30_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({8.0_ft, 12_ft-5_ft}, 180_deg-135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({7.7_ft, 12_ft-4.5_ft}, 180_deg-180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({7.7_ft, 12_ft-3_ft}, 180_deg-180_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({10.5_ft, 12_ft-1.5_ft}, 180_deg-135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
  driveToPoint({12_ft, 12_ft-0_ft}, 180_deg-135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(1_s, TimeUtilFactory().create()));
  placeStack(1500);
  rollerOuttake(0.3);
  driveToPoint({9.5_ft, 12_ft-2.5_ft}, 180_deg-135_deg, 1, Settler().distanceErr(4_in).angleErr(5_deg).maxTime(5_s, TimeUtilFactory().create()));
}
void threeCubeBigRed() {}
void threeCubeBigBlue() {}
void fourCubeBigRed() {}
void fourCubeBigBlue() {}
