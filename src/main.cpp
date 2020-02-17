#include "main.h"
#include "lib7842/api.hpp"
#include "subsystems/arm.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter2.hpp"
#include "ui/autonselector.hpp"
#include "util/timedelayedboolean.hpp"
#include "util/util.hpp"

using namespace lib7842;
using namespace lib7842::units;
using namespace okapi;

Controller controller{ControllerId::master};
Controller partner{ControllerId::partner};

auto topLeft = std::make_shared<Motor>(4);
auto topRight = std::make_shared<Motor>(-5);
auto bottomRight = std::make_shared<Motor>(-7);
auto bottomLeft = std::make_shared<Motor>(6);
// This is the model of the robot chassis
// It keeps track of the motors, encoder wheels, max Velocity (in RPM), and max
// Voltage
auto model{std::make_shared<ThreeEncoderXDriveModel>(
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
    )};

auto odomChassisScales = ChassisScales(
    {2.792_in, // Encoder wheel diameter
     10_in,    // Drive base width, assumes encoder wheels are the same distance
               // from the center
     0.5_in},  // Middle wheel distance, which doesn't matter to the math
    360);      // Encoder ticks per rotstion
auto driveChassisScales = ChassisScales(
    {4_in,  // Encoder wheel diameter
     9_in,  // Drive base width, assumes encoder wheels are the same distance
            // from the center
     0_in}, // Middle wheel distance, which doesn't matter to the math
    imev5GreenTPR); // Encoder ticks per rotstion
// keeps track of position and orientation based on bot measurements
auto odom{std::make_shared<CustomOdometry>(model, odomChassisScales,
                                           TimeUtilFactory().create())};

GUI::Screen scr{lv_scr_act(), LV_COLOR_RED};

auto odomController{std::make_shared<OdomXController>(
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
PursuitLimits halfSpeedLimits{0.2_mps,  1.1_mps2, 0.75_mps,
                              0.4_mps2, 0_mps,    40_mps};

// replace with a motor group
auto stackerMotor1{
    std::make_shared<Motor>(Constants::STACKER_TREAD_1_MOTOR_PORT)};
auto stackerMotor2{
    std::make_shared<Motor>(Constants::STACKER_TREAD_2_MOTOR_PORT)};

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
// Constant distances used for autonomous plotting
const QLength botWidth{17.5_in};
const QLength botLength{17.5_in};

const QLength cubeWidth{5.5_in};
const QLength fieldWidth{6_tile};
const QLength towerBaseWidth{7.8_in};
const QLength zoneWidth{10_in};
const QLength bigZoneLength{15.5_in};
const QLength barrierWidth{2_in};

// These are used to determine which autonomous is run
// Currently are updated based on the autonomous selector
AutonSelector::Color alliance{AutonSelector::getColor()};
AutonSelector::Auton currentAuton{AutonSelector::getAuton()};

LatchedBoolean tilterOn{};
LatchedBoolean tilterOff{};

void initialize() {
  pros::delay(200);

  // Start UI
  odom->startTask("Odometry");
  scr.makePage<AutonSelector>("Autonomous Selector");
  scr.makePage<GUI::Odom>("Odometry").attachOdom(odom);
  scr.startTask("Screen");
  initializeArm();
  initializeTilter();
  // prevent up at start
  tilterOff.update(true);
  tilterOn.update(true);
}

void disabled() {}

void competition_initialize() {}

// Wraps the odomController calls for easier autonomous programming
// Takes a target position and orientation

void driveToPoint(Vector target, QAngle targetAngle = 0_deg,
                  double turnPriority = 1, Settler&& settler = Settler().distanceErr(4_in).angleErr(5_deg)){
  odomController->strafeToPoint(
      target, OdomController::makeAngleCalculator(targetAngle), turnPriority,
      std::move(settler));
}

// Same as above, but faces a point during the motion instead of a set
// orientation
void driveToPoint(Vector target, Vector pointToFace, double turnPriority = 1,
                  QLength settleDistance = 4_in, QAngle settleAngle = 5_deg) {

  odomController->strafeToPoint(
      target, OdomController::makeAngleCalculator(pointToFace), turnPriority,
      Settler().distanceErr(settleDistance).angleErr(settleAngle));
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

void ppTest() {
  auto path = SimplePath({odom->getState(),
                          {0_ft, 0_ft},
                          {0_ft, 2_ft},
                          {2_ft, 2_ft},
                          {2_ft, 4_ft}})
                  .generate(1_cm)
                  .smoothen(.001, 1e-10 * meter);

  follower.followPath(PathGenerator::generate(path, fullSpeedLimits));
}
void autonomous() {
  odom->reset();
  model->setMaxVoltage(12000);
  // Get the chosen alliance and auton
  alliance = AutonSelector::getColor();
  currentAuton = AutonSelector::getAuton();
  stackerMotor1->moveRelative(600, 12000);
  stackerMotor2->moveRelative(600, 12000);
  pros::delay(200);
  setTilterDown();
  // setArmLowMiddle();
  switch (currentAuton) {
  case AutonSelector::Auton::TEST: {
    sixCubeBlue();
    //			switch(alliance){
    //				case AutonSelector::Color::RED:{
    //					sixCubeRed();
    //				}
    //				case AutonSelector::Color::BLUE:{
    //					sixCubeBlue();
    //				}
    //			}
    break;
  }
  case AutonSelector::Auton::SMALL_ZONE_ONE_CUBE: {
    break;
  }
  case AutonSelector::Auton::BIG_ZONE_ONE_CUBE: {
    break;
  }
  case AutonSelector::Auton::SMALL_ZONE_5STACK: {

    break;
  }
  case AutonSelector::Auton::SMALL_ZONE_6STACK: {
    switch (alliance) {
    case AutonSelector::Color::RED: {
      sixCubeRed();
    }
    case AutonSelector::Color::BLUE: {
      sixCubeBlue();
    }
    }
    break;
  }
  case AutonSelector::Auton::SMALL_ZONE_7STACK: {
    break;
  }
  case AutonSelector::Auton::SMALL_ZONE_8STACK: {
    break;
  }
  case AutonSelector::Auton::BIG_ZONE_3STACK: {
    rollerIntake();
    // drive forward, grab cube
    driveToPoint({0_in, 1_tile});
    // turn around and grab cube by zone
    // slow intake
    // may need to tune Y value
    rollerOuttake(-0.7);

    model->setMaxVoltage(10000);
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-6_in, 36_in}, -135_deg);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({6_in, 36_in}, 135_deg);
      break;
    }
    }
    // line up
    // definitely needs tuned
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-31_in, 8_in}, -135_deg);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({30_in, 10_in}, 135_deg);
      break;
    }
    }

    // place
    setTilterUp();
    pros::delay(2000);
    model->setMaxVoltage(4000);
    // back up
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-31_in + 15_in, 8_in + 15_in}, -135_deg);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg);
      break;
    }
    }
    break;
    setTilterDown();
  }
  case AutonSelector::Auton::BIG_ZONE_LARGESTACK: {
    driveToPoint({0_in, 14_in});
    // deploy tray
    driveToPoint({0_in, 1_in});
    rollerOuttake();
    pros::delay(1700);
    // intake
    rollerIntake();
    // drive forward, grab cube
    model->setMaxVoltage(10000);
    driveToPoint({0_in, 1.5_tile});
    driveToPoint({0_in, 1.4_tile});
    model->setMaxVoltage(6000);
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-14_in, 1.5_tile}, 0_deg, 1);
      rollerOuttake(-0.75);
      driveToPoint({-14_in, 2_tile}, 0_deg, 1);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({12_in, 1.5_tile}, 0_deg, 1);
      rollerOuttake(-0.75);
      driveToPoint({12_in, 2_tile}, 0_deg, 1);
      break;
    }
    }
    // turn around and grab cube by zone
    // slow intake
    // may need to tune Y value
    rollerOuttake(-0.3);

    model->setMaxVoltage(8000);
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-6_in + 10_in, 36_in + 10_in}, -135_deg);

      rollerIntake();
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({6_in, 36_in}, 135_deg);
      break;
    }
    }
    // line up
    // definitely needs tuned
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-28.25_in, 7.25_in}, -135_deg);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({30_in, 10_in}, 135_deg);
      break;
    }
    }
    rollerStop();
    // place
    setTilterUp();
    pros::delay(2000);
    model->setMaxVoltage(4000);
    // back up
    switch (alliance) {
    case AutonSelector::Color::RED: {
      driveToPoint({-28_in + 15_in, 7.5_in + 15_in}, -135_deg);
      break;
    }
    case AutonSelector::Color::BLUE: {
      driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg);
      break;
    }
    }
    break;
    setTilterDown();
  }
  case AutonSelector::Auton::BIG_ZONE_PUSH: {
    break;
  }
  default:
    break;
  }
}

Stacker stacker{*Stacker::getInstance()};

// used for toggle presses
LatchedBoolean left{};
LatchedBoolean right{};
LatchedBoolean offsetPressForward{};
LatchedBoolean offsetPressBackward{};
LatchedBoolean toggleSpeed{};
// LatchedBoolean resetArms = LatchedBoolean();
DelayedBoolean resetArms = DelayedBoolean(pros::millis(), 1000);
bool tilterUp = false;
bool reset = true;

void opcontrol() {
  model->setMaxVoltage(12000);
  while (true) {

    // maps the drive inputs into curves, making them easier to finely control
    double turn =
        Util::map(Util::curveJoystick(
                      false,
                      Util::map(controller.getAnalog(ControllerAnalog::leftX),
                                -1, 1, -127, 127),
                      15),
                  -127, 127, -1, 1);
    double xPower =
        Util::map(Util::curveJoystick(
                      false,
                      Util::map(controller.getAnalog(ControllerAnalog::rightX),
                                -1, 1, -127, 127),
                      10),
                  -127, 127, -1, 1);
    double yPower =
        Util::map(Util::curveJoystick(
                      false,
                      Util::map(controller.getAnalog(ControllerAnalog::rightY),
                                -1, 1, -127, 127),
                      10),
                  -127, 127, -1, 1);

    // drives the bot
    model->xArcade(xPower, yPower, turn);
    stackerMotor1->moveVoltage(stacker.getOutput());
    stackerMotor2->moveVoltage(stacker.getOutput());

    // control the rollers
    if (controller.getDigital(ControllerDigital::R1) &&
        controller.getDigital(ControllerDigital::R2)) {
      stackerMotor1->moveVoltage(8000);
      stackerMotor2->moveVoltage(-8000);
    } else if (controller.getDigital(ControllerDigital::R1)) {
      stacker.outtake();
      stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
      stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
    } else if (controller.getDigital(ControllerDigital::R2)) {
      stacker.intake();
      stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
      stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
    } else {
      stacker.stop();
      stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::hold);
      stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::hold);
    }

    stacker.in();
    stacker.out();

    bool tilterPress =
        tilterOn.update(controller.getDigital(ControllerDigital::L1));
    bool tilterRelease =
        tilterOff.update(!controller.getDigital(ControllerDigital::L1));
    if (tilterPress && !tilterUp) {
      // set middle
      setTilterMiddle();
    }
    if (tilterRelease) {
      if (tilterUp) {
        // set down
        setTilterDown();
        reset = false;
        tilterUp = false;
      } else {
        // set up
        setTilterUp();
        stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
        stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
        tilterUp = true;
        setArmDown();
      }
    }
    if (controller.getDigital(ControllerDigital::X)) {
      setTilterManeuver();
    }

    reset = resetArms.update(pros::millis(), !tilterUp);

    if (partner.getDigital(ControllerDigital::left)) {
      armPID->flipDisable(true);
      setTilterDisabled();
      setArmManualSpeed(1);
    } else if (partner.getDigital(ControllerDigital::up)) {
      armPID->flipDisable(true);
      setTilterDisabled();
      setArmManualSpeed(-1);
    } else if (partner.getDigital(ControllerDigital::down)) {
      armPID->flipDisable(false);
      setTilterDisabled();
      setArmDown();
    } else if (partner.getDigital(ControllerDigital::L2)) {
      armPID->flipDisable(false);
      setTilterTowers();
      setArmAlliance();
      tilterUp = true;
    } else if (partner.getDigital(ControllerDigital::right)) {
      armPID->flipDisable(false);
      setTilterTowers();
      setArmHighMiddle();
      tilterUp = true;
    } else if (reset) {
      armPID->flipDisable(false);
      setArmLowMiddle();
    }

    // slow the speed of the drivetrain for moving stacks
    if (toggleSpeed.update(controller.getDigital(ControllerDigital::Y))) {
      if (model->getMaxVoltage() == 12000) {
        model->setMaxVoltage(4000);
      } else {
        model->setMaxVoltage(12000);
      }
    }
    pros::delay(10);
  }
}
