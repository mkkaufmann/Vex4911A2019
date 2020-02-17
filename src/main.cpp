#include "main.h"
#include "auton.hpp"
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

GUI::Screen scr{lv_scr_act(), LV_COLOR_RED};

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
    break;
  }
  case AutonSelector::Auton::one: {
    oneCube();
    break;
  }
  case AutonSelector::Auton::five: {
    switch (alliance) {
    case AutonSelector::Color::RED: {
      fiveCubeRed();
    }
    case AutonSelector::Color::BLUE: {
      fiveCubeBlue();
    }
    }
    break;
  }
  case AutonSelector::Auton::six: {
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
  case AutonSelector::Auton::seven: {
    switch (alliance) {
    case AutonSelector::Color::RED: {
      sevenCubeRed();
    }
    case AutonSelector::Color::BLUE: {
      sevenCubeBlue();
    }
    }
    break;
  }
  case AutonSelector::Auton::nine: {
    switch (alliance) {
    case AutonSelector::Color::RED: {
      nineCubeRed();
    }
    case AutonSelector::Color::BLUE: {
      nineCubeBlue();
    }
    }
    break;
  }
  case AutonSelector::Auton::THREE: {
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
  case AutonSelector::Auton::FOUR: {
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
