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

enum Auton{
		ONE_CUBE,
		RED_RIGHT_ONE_CUBE_AND_LINE,
		RED_RIGHT_LINE_AND_PLACE,
		BLUE_LEFT_ONE_CUBE_AND_LINE,
		BLUE_LEFT_LINE_AND_PLACE
};
	
Auton auton = RED_RIGHT_LINE_AND_PLACE;

//starting orientations
double facingForward = Util::toRadians(0);
double facingLeft = Util::toRadians(90);//not sure
double facingRight = Util::toRadians(180);
double facingBackward = Util::toRadians(270);

//initialize static values

//ports
int Constants::LEFT_FRONT_MOTOR_PORT = 4;
int Constants::LEFT_REAR_MOTOR_PORT = 2;
int Constants::RIGHT_FRONT_MOTOR_PORT = -3;
int Constants::RIGHT_REAR_MOTOR_PORT = -1;
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



        Drive drive = *Drive::getInstance();



		using namespace okapi; 
		auto topLeft = drive.leftFrontMotor;
		auto topRight = drive.rightFrontMotor;
		auto bottomRight = drive.rightRearMotor;
		auto bottomLeft = drive.leftRearMotor;
		auto drivetrain = okapi::ChassisControllerFactory::create(
						topLeft, 
						topRight, 
						bottomRight, 
						bottomLeft, 
						okapi::AbstractMotor::gearset::green, 
						{4 * okapi::inch, 13 * okapi::inch}
						);
		
		auto profileFollower =okapi::AsyncControllerFactory::motionProfile(
						0.4,
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

		switch(auton){
				case ONE_CUBE:		
						profileFollower.generatePath({
								okapi::Point{0_ft, 0_ft, 0_deg},
								okapi::Point{18_in, 0_ft, 0_deg}},
								"PushCube"
						);
						break;
				case RED_RIGHT_ONE_CUBE_AND_LINE:	
						profileFollower.generatePath({
								okapi::Point{0_ft, 0_ft, 0_deg},
								okapi::Point{18_in, 0_ft, 0_deg}},
								"PushCube"
						);	
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{10_in, 0_in, 0_deg}},
								"LineUp"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{30_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{20_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						break;
				case RED_RIGHT_LINE_AND_PLACE:
						
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{34_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{22_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{6_in, 0_in, 0_deg}},
								"DriveIntoZone"
						);
						break;
				case BLUE_LEFT_ONE_CUBE_AND_LINE:

						profileFollower.generatePath({
								okapi::Point{0_ft, 0_ft, 0_deg},
								okapi::Point{18_in, 0_ft, 0_deg}},
								"PushCube"
						);	
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{9_in, 0_in, 0_deg}},
								"LineUp"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{30_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::Point{0_in, 0_ft, 0_deg},
								okapi::Point{22_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						break;
				case BLUE_LEFT_LINE_AND_PLACE:

						break;
		}
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
void competition_initialize() {
		
}
 Tilter tilter = *Tilter::getInstance();
		auto stacker1 = okapi::Motor(6);
		auto stacker2 = okapi::Motor(-5);
void autonomous(){

		using namespace okapi;
		stacker1.move(127);
		stacker2.move(127);
		
		switch(auton){
				case ONE_CUBE:
						profileFollower.setTarget("PushCube", true);
						profileFollower.waitUntilSettled();
						profileFollower.setTarget("PushCube", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("PushCube");
						break;
				case RED_RIGHT_ONE_CUBE_AND_LINE:
						profileFollower.setTarget("PushCube", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("PushCube");
						profileFollower.setTarget("LineUp", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("LineUp");
						stacker1.move(-127 * 0.8);
						stacker2.move(-127 * 0.8);
						drivetrain.turnAngle(std::sqrt(2) * (115_deg));
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						stacker1.move(-127 * 0.0);
						stacker2.move(-127 * 0.0);
						drivetrain.turnAngle(std::sqrt(2) * (170_deg));
						break;
				case RED_RIGHT_LINE_AND_PLACE:
						stacker1.move(127);
						stacker1.move(127);
						pros::delay(1500);
						stacker1.move(-127 * 0.8);
						stacker2.move(-127 * 0.8);
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						drivetrain.turnAngle(std::sqrt(2) * (200_deg));
						profileFollower.setTarget("DriveIntoZone", false);
						profileFollower.waitUntilSettled();

						profileFollower.removePath("DriveAwayFromLine");
						tilter.in();
						tilter.shiftUp();
						//manually set

						tilter.out();
						break;
				case BLUE_LEFT_ONE_CUBE_AND_LINE:

						profileFollower.setTarget("PushCube", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("PushCube");
						profileFollower.setTarget("LineUp", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("LineUp");
						stacker1.move(-127 * 0.8);
						stacker2.move(-127 * 0.8);
						drivetrain.turnAngle(std::sqrt(2) * (-115_deg));
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						stacker1.move(-127 * 0.0);
						stacker2.move(-127 * 0.0);
						drivetrain.turnAngle(std::sqrt(2) * (-170_deg));
						break;
				case BLUE_LEFT_LINE_AND_PLACE:

						break;
		}

		//if(isLine){

				//stacker1.move(-127 * 0.6);
				//stacker2.move(-127 * 0.6);
				
/*				topLeft.moveRelative(700, 127*0.2);
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
*/				
		//}		
}

 PositionTracker tracker = *PositionTracker::getInstance();
 Stacker stacker = *Stacker::getInstance();

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
