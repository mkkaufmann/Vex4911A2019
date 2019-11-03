#include "main.h"
#include "constants.hpp"
#include "subsystems/oi.hpp"
#include "subsystems/positiontracker.hpp"
#include "subsystems/drive.hpp"
#include "subsystems/stacker.hpp"
#include "motionprofiles/pathfollower.hpp"
#include "util/point.hpp"
#include "util/latchedboolean.hpp"
#include "util/util.hpp"
#include "util/actions.hpp"
#include "pros/apix.h"
#include "ui/ui.hpp"
#include "ui/uihelper.hpp"
#include "util/autontimer.hpp"
#include "subsystems/tilter.hpp"


//edit this
bool isLeft = true;
bool isLine = true;

//starting orientations
double facingForward = Util::toRadians(0);
double facingLeft = Util::toRadians(90);//not sure
double facingRight = Util::toRadians(180);
double facingBackward = Util::toRadians(270);

//initialize static values

//ports
int Constants::LEFT_FRONT_MOTOR_PORT = -1;
int Constants::LEFT_REAR_MOTOR_PORT = -2;
int Constants::RIGHT_FRONT_MOTOR_PORT = 3;
int Constants::RIGHT_REAR_MOTOR_PORT = 4;
int Constants::STACKER_TREAD_2_MOTOR_PORT = 5;
int Constants::STACKER_TREAD_1_MOTOR_PORT = 6;
int Constants::TILTER_MOTOR_PORT = 7;

int Constants::LEFT_TRACKING_ENCODER_TOP = 1;
int Constants::LEFT_TRACKING_ENCODER_BOTTOM = 2;
int Constants::RIGHT_TRACKING_ENCODER_TOP = 3;
int Constants::RIGHT_TRACKING_ENCODER_BOTTOM = 4;
int Constants::REAR_TRACKING_ENCODER_TOP = 5;
int Constants::REAR_TRACKING_ENCODER_BOTTOM = 6;

//measured constants for odometry
double Constants::LEFT_WHEEL_TO_TRACKING_CENTER = 6.5;
double Constants::RIGHT_WHEEL_TO_TRACKING_CENTER = 7.25;
double Constants::REAR_WHEEL_TO_TRACKING_CENTER = 7;
int Constants::ENCODER_TICKS_PER_INCH = 360/(2.75 * M_PI);

//initialize singletons
OI* OI::instance = OI::getInstance();
Drive* Drive::instance = Drive::getInstance();
Tilter* Tilter::instance = Tilter::getInstance();
Stacker* Stacker::instance = Stacker::getInstance();
PositionTracker* PositionTracker::instance = PositionTracker::getInstance();

//initialize odometry values
double PositionTracker::pLeftEnc = 0;
double PositionTracker::pRightEnc = 0;
double PositionTracker::pBackEnc = 0;
double PositionTracker::pTheta = facingForward;//change this to starting orientation
double PositionTracker::leftEncAtLastReset = 0;
double PositionTracker::rightEncAtLastReset = 0;
double PositionTracker::thetaAtLastReset = PositionTracker::pTheta;
Point PositionTracker::globalPosition = Point(0,0);

//set initial drive setting
bool Drive::fieldCentric = false;
//used for changing the above setting during a match
LatchedBoolean Drive::fieldCentricToggle = LatchedBoolean();

//number of loops for PID to count as settled
double PathFollower::iterationsForSettle = 5;//100ms

int AutonTimer::startTime = -1;






		using namespace okapi; 
		auto topLeft = okapi::Motor(4);
		auto topRight = okapi::Motor(-1);
		auto bottomRight = okapi::Motor(-2);
		auto bottomLeft = okapi::Motor(3);
		auto drivetrain = okapi::ChassisControllerFactory::create(
						topLeft, 
						topRight, 
						bottomRight, 
						bottomLeft, 
						okapi::AbstractMotor::gearset::green, 
						{4 * okapi::inch, 13 * okapi::inch}
						);
		
		auto profileFollower =okapi::AsyncControllerFactory::motionProfile(
						0.5,
						1.0,
						5.0,
						drivetrain
						);


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
 UIHelper::initialize();
		profileFollower.generatePath({
						okapi::Point{0_ft, 0_ft, 0_deg},
						okapi::Point{(isLeft?1.7_ft:1.4_ft), (isLeft?0.2_ft:-1_ft), 0_deg}},
						"B"
						);
		profileFollower.generatePath({
						okapi::Point{0_ft, 0_ft, 0_deg},
						okapi::Point{6.5_in, 0_ft, 0_deg}},

						"A"
						);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

		auto stacker1 = okapi::Motor(6);
		auto stacker2 = okapi::Motor(-5);
        Drive drive = *Drive::getInstance();
void autonomous(){

		using namespace okapi;
		
		profileFollower.setTarget("B", isLeft);
		profileFollower.waitUntilSettled();
	    profileFollower.removePath("B");
		profileFollower.setTarget("A", !isLeft);
		profileFollower.waitUntilSettled();
	    profileFollower.removePath("A");

		stacker1.move(127);
		stacker2.move(127);
		pros::delay(2000);
		
		//motors are messed up
		//topLeft is top Left, + is forward 
		//topRight is bottom right, + is forward 
		//bottomLeft is top right, - is forward 
		//bottomRight is bottom left, - is forward 
		//
		if(isLine){
				stacker1.move(-127 * 0.6);
				stacker2.move(-127 * 0.6);
				topLeft.moveRelative(700, 127*0.2);
				topRight.moveRelative(700, 127*0.2);
				bottomLeft.moveRelative(-700, 127*0.2);
				bottomRight.moveRelative(-700, 127*0.2);
				pros::delay(5000);
				topLeft.moveRelative(-420, 50);
				topRight.moveRelative(-420, 50);
				bottomLeft.moveRelative(420, 50);
				bottomRight.moveRelative(420, 50);
				pros::delay(2000);

				drivetrain.turnAngle((isLeft?-180:180) * std::sqrt(2) * okapi::degree);
				pros::delay(1200);
				topLeft.moveRelative(50, 127*0.2);
				topRight.moveRelative(50, 127*0.2);
				bottomLeft.moveRelative(50, 127*0.2);
				bottomRight.moveRelative(50, 127*0.2);
				
		}		
}

 PositionTracker tracker = *PositionTracker::getInstance();
 Stacker stacker = *Stacker::getInstance();
 Tilter tilter = *Tilter::getInstance();
 OI oi = *OI::getInstance();

 LatchedBoolean left = LatchedBoolean();
 LatchedBoolean right = LatchedBoolean();
 pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
 pros::Controller partner = pros::Controller(pros::E_CONTROLLER_PARTNER);

void runSubsystems(){
  //update drive output
  drive.driveManually(-master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_RIGHT_Y), master.get_analog(ANALOG_LEFT_X));
  drive.updateFieldCentric(master.get_digital(DIGITAL_B));
  //run every subsystem
  stacker.in();
//  stacker.stateChangeRequest(master.get_digital(DIGITAL_L1), master.get_digital(DIGITAL_L2));
  if(master.get_digital(DIGITAL_LEFT)){
				  stacker.slowOuttake();
  }else if(master.get_digital(DIGITAL_R1)){
		  stacker.outtake();
  }else if(master.get_digital(DIGITAL_R2)){
		  stacker.intake();
  }else{
		stacker.stop();
  }

  stacker.out();
  stacker1.move(stacker.getOutput());
  stacker2.move(stacker.getOutput());
  drive.in();
  drive.out();
  tracker.in();
  tracker.out();
  if(left.update(master.get_digital(DIGITAL_L2))){
    tilter.shiftDown();
  }
  if(right.update(master.get_digital(DIGITAL_L1))){
    tilter.shiftUp();
  }
  if(master.get_digital(DIGITAL_UP)){
		tilter.adjustThrottle(127*0.5);
  }else if(master.get_digital(DIGITAL_DOWN)){
		tilter.adjustThrottle(-127 * 0.5);
  }else{
		tilter.adjustThrottle(0);
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
