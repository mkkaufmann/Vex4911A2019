#include "main.h"
#include "subsystems/positiontracker.hpp"
#include "subsystems/drive.hpp"
#include "constants.hpp"
#include "subsystems/subsystemmanager.hpp"
#include "subsystems/oi.hpp"
#include "util/util.hpp"
#include "subsystems/stacker.hpp"
#include "ui/uihelper.hpp"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
 Drive drive = *Drive::getInstance();
 PositionTracker tracker = *PositionTracker::getInstance();
 Stacker stacker = *Stacker::getInstance();
 OI oi = *OI::getInstance();

 pros::Motor stackerMotor = pros::Motor(Constants::STACKER_TREAD_MOTOR_PORT);
 pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

void runSubsystems(){
  //update drive output
  drive.driveManually(-master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y), master.get_analog(ANALOG_LEFT_X));
  drive.updateFieldCentric(master.get_digital(DIGITAL_B));
  //run every subsystem
  stacker.in();
  stacker.stateChangeRequest(master.get_digital(DIGITAL_UP), master.get_digital(DIGITAL_DOWN));
  stacker.out();
  stackerMotor.move(stacker.getOutput());
  drive.in();
  drive.out();
  tracker.in();
  tracker.out();
}

void updateScreen(){
  UIHelper::updateDisplay(
    tracker.getLEncValue(),
    tracker.getREncValue(),
    tracker.getBEncValue(),
    tracker.getGlobalPosition().getX(),
    tracker.getGlobalPosition().getY(),
    tracker.getTheta()
  );
}

void opcontrol() {
	while (true) {
    runSubsystems();
    updateScreen();
		pros::delay(20);
	}
}
