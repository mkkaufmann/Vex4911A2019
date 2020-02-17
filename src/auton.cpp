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
std::shared_ptr<ThreeEncoderXDriveModel> model = std::make_shared<ThreeEncoderXDriveModel>(
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
std::shared_ptr<CustomOdometry> odom{std::make_shared<CustomOdometry>(model, odomChassisScales,
                                           TimeUtilFactory().create())};

std::shared_ptr<OdomXController> odomController{std::make_shared<OdomXController>(
    model, odom,
    // Distance PID - To mm
    // Used for strafing and driving
    std::make_unique<IterativePosPIDController>(
        0.0178*1.5, 0.000001, 0.0009, 0,
        TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    // Turn PID - To Degree
    // Used for turning
    std::make_unique<IterativePosPIDController>(
        0.03, 0.00, 0.0003, 0,
        TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    // Angle PID - To Degree
    // Used for turning
    std::make_unique<IterativePosPIDController>(
        0.02, 0, 0.0, 0, TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
    TimeUtilFactory().create())};

PathFollower follower(model, odom, driveChassisScales, 1_ft,
                      TimeUtilFactory().create());

PursuitLimits fullSpeedLimits{0.2_mps,  1.1_mps2, 0.75_mps,
                              0.4_mps2, 0_mps,    40_mps};
PursuitLimits halfSpeedLimits{0.1_mps,  1.1_mps2/2, 0.75_mps/2,
                              0.4_mps2/2, 0_mps,    40_mps/2};

// Wraps the odomController calls for easier autonomous programming
// Takes a target position and orientation

void driveToPoint(Vector target, QAngle targetAngle,
                  double turnPriority, Settler&& settler){
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

// incorrect stack position
void sixCubeRed() {

  // full speed to line
  driveToPoint({0_in, 12_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(8000);
  driveToPoint({0_in, 15_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 20_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(5300);
  driveToPoint({0_in, 40_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // rest of line
  model->setMaxVoltage(4000);
  driveToPoint({0_in, 48_in}, 0_deg);

  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({15_in, 12_in}, 135_deg, 2);
  setTilterMiddle();
  driveToPoint({16_in, 9_in}, 135_deg, 1, Settler().distanceErr(2_in).angleErr(5_deg));
  setTilterUp();
  pros::delay(2000);
  rollerOuttake(0.5);
  driveToPoint({0_in, 21_in}, 135_deg, 2, Settler().distanceErr(1_in).angleErr(2_deg));
}

void eightCubeRed() {

  setArmLowMiddle();
  // full speed to line
  driveToPoint({0_in, 13_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(5000);
  driveToPoint({0_in, 48_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(12000);
  driveToPoint({20_in, 9_in}, -30_deg, 1.5, Settler().distanceErr(7_in).angleErr(5_deg));
  driveToPoint({20_in, 9_in}, 0_deg, 1.5, Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(6000);
  driveToPoint({20_in, 30_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  model->setMaxVoltage(4000);
  driveToPoint({20_in, 48_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  pros::delay(350);
  stackerMotor1->moveRelative(300, 12000);
  stackerMotor2->moveRelative(300, 12000);
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({21_in, 17_in}, 135_deg, 1);
  driveToPoint({27_in, 15_in}, 135_deg, 1);
  driveToPoint({31_in, 7_in}, 135_deg, 1, Settler().distanceErr(6_in).angleErr(90_deg));
  setArmDown();
  setTilterUp();
  pros::delay(2200);
  rollerOuttake(0.5);
  driveToPoint({31_in - 10_in, 7_in + 10_in}, 135_deg, 1, Settler().distanceErr(2_in).angleErr(5_deg));
}

void eightCubeBlue() {

  setArmLowMiddle();
  // full speed to line
  driveToPoint({0_in, 13_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(5000);
  driveToPoint({0_in, 48_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(12000);
  driveToPoint({-20_in, 9_in}, 30_deg, 1.5, Settler().distanceErr(7_in).angleErr(5_deg));
  driveToPoint({-20_in, 9_in}, 0_deg, 1.5, Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(6000);
  driveToPoint({-20_in, 30_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  model->setMaxVoltage(4000);
  driveToPoint({-20_in, 48_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  pros::delay(350);
  stackerMotor1->moveRelative(300, 12000);
  stackerMotor2->moveRelative(300, 12000);
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({-21_in, 17_in}, -135_deg, 1);
  driveToPoint({-27_in, 15_in}, -135_deg, 1);
  driveToPoint({-31_in, 7_in}, -135_deg, 1, Settler().distanceErr(6_in).angleErr(90_deg));
  setArmDown();
  setTilterUp();
  pros::delay(2200);
  rollerOuttake(0.5);
  driveToPoint({-(31_in - 10_in), 7_in + 10_in}, -135_deg, 1, Settler().distanceErr(2_in).angleErr(5_deg));
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

void sixCubeBlue() {

  // full speed to line
  driveToPoint({0_in, 12_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  rollerIntake();
  // first cube
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 15_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // second cube
  model->setMaxVoltage(6000);
  driveToPoint({0_in, 20_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // third cube
  model->setMaxVoltage(5300);
  driveToPoint({0_in, 40_in}, 0_deg, 1, Settler().distanceErr(7_in).angleErr(5_deg));
  // rest of line
  model->setMaxVoltage(6000);
  driveToPoint({0_in, 44_in}, 0_deg, 1, Settler().distanceErr(5_in).angleErr(30_deg));
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({-12_in, 7_in}, -135_deg, 2);
  setTilterMiddle();
  stackerMotor1->moveRelative(600, 12000);
  stackerMotor2->moveRelative(600, 12000);
  driveToPoint({-14_in, 5_in}, -135_deg, 1, Settler().distanceErr(2_in).angleErr(5_deg));
  setTilterUp();
  pros::delay(2000);
  rollerOuttake(0.5);
  driveToPoint({0_in, 21_in}, -135_deg, 2, Settler().distanceErr(1_in).angleErr(2_deg));
}
void nineCubeRed() {
  model->setMaxVoltage(12000);
  driveToPoint({0_in, 19_in});
  rollerIntake();
  model->setMaxVoltage(6000);
  driveToPoint({0_in, 45_in});
  model->setMaxVoltage(12000);
  driveToPoint({13_in, 40_in});
  model->setMaxVoltage(8000);
  driveToPoint({13_in, 55_in});
  model->setMaxVoltage(8000);
  driveToPoint({25_in, 55_in}, 90_deg);
  model->setMaxVoltage(8000);
  driveToPoint({25_in, 40_in}, 90_deg, 2);
  model->setMaxVoltage(12000);
  driveToPoint({27_in, 50_in}, 180_deg, 0.5);
  model->setMaxVoltage(4000);
  driveToPoint({27_in, 10_in}, 180_deg);
  //			driveToPoint({23.5_in, 12_in}, 180_deg);
}
