#include "main.h"
#include "subsystems/positiontracker.hpp"
#include "subsystems/drive.hpp"
#include "constants.hpp"
#include "subsystems/subsystemmanager.hpp"
#include "subsystems/oi.hpp"
#include "util/util.hpp"
#include "subsystems/stacker.hpp"
#include "ui/uihelper.hpp"
#include "subsystems/tilter.hpp"
  
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
 Tilter tilter = *Tilter::getInstance();
 OI oi = *OI::getInstance();

 LatchedBoolean left = LatchedBoolean();
 LatchedBoolean right = LatchedBoolean();
 pros::Motor stacker1Motor = pros::Motor(Constants::STACKER_TREAD_1_MOTOR_PORT);
 pros::Motor stacker2Motor = pros::Motor(Constants::STACKER_TREAD_2_MOTOR_PORT);
 pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
 pros::Controller partner = pros::Controller(pros::E_CONTROLLER_PARTNER);

void runSubsystems(){
  //update drive output
  drive.driveManually(-master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y), master.get_analog(ANALOG_LEFT_X));
  drive.updateFieldCentric(master.get_digital(DIGITAL_B));
  //run every subsystem
  stacker.in();
  stacker.stateChangeRequest(master.get_digital(DIGITAL_L1), master.get_digital(DIGITAL_L2));
  stacker.out();
  stacker1Motor.move(stacker.getOutput());
  stacker2Motor.move(-stacker.getOutput());
  drive.in();
  drive.out();
  tracker.in();
  tracker.out();
  if(left.update(master.get_digital(DIGITAL_R1))){
    tilter.shiftDown();
  }
  if(right.update(master.get_digital(DIGITAL_R2))){
    tilter.shiftUp();
  }
  tilter.in();
  tilter.out();
  if(master.get_digital(DIGITAL_X)){
		tracker.resetRotation();
  }
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
