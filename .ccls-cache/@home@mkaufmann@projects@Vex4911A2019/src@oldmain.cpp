#include <cstdio>
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
//#include "ui/ui.hpp"
//#include "ui/uihelper.hpp"
#include "util/autontimer.hpp"
#include "subsystems/tilter.hpp"
#include "ui/wrapper/object.hpp"
#include "ui/wrapper/button.hpp"
#include "ui/wrapper/label.hpp"
enum Auton{
		ONE_CUBE_RIGHT,
		ONE_CUBE_LEFT,
		RED_RIGHT_ONE_CUBE_AND_LINE,
		RED_RIGHT_LINE_AND_PLACE,
		BLUE_LEFT_ONE_CUBE_AND_LINE,
		BLUE_LEFT_LINE_AND_PLACE,
		RED_GIRL_POWER,
		BLUE_GIRL_POWER
};
	
Auton auton = ONE_CUBE_LEFT;



using namespace okapi; 
using namespace lib7842;
Drive drive = *Drive::getInstance();
auto topLeft = drive.leftFrontMotor;
auto topRight = drive.rightFrontMotor;
auto bottomRight = drive.rightRearMotor;
auto bottomLeft = drive.leftRearMotor;
//		auto drivetrain = okapi::ChassisControllerFactory::create(
//						topLeft, 
//						topRight, 
//						bottomRight, 
//						bottomLeft, 
//						okapi::AbstractMotor::gearset::green, 
//						{4 * okapi::inch, 13 * okapi::inch}
//						);
		
  auto model = std::make_shared<ThreeEncoderXDriveModel>(
    // motors
    std::make_shared<Motor>(4), //topLeft
    std::make_shared<Motor>(-3), //topRight
    std::make_shared<Motor>(-1), //bottomRight
    std::make_shared<Motor>(2), //bottomLeft
    // sensors
    std::make_shared<ADIEncoder>(1, 2), // left
    std::make_shared<ADIEncoder>(3, 4), // right
    std::make_shared<ADIEncoder>(5, 6), // middle
    // limits
    200, 12000);//maxVel, maxVoltage


		const int screenWidth = 480;
		const int screenHeight = 240;
  static lv_style_t container_style;

  Object container = Object::create()
		  .setPosition(0, 0)
		  .setSize(screenWidth,screenHeight);
//		  .setStyle(&container_style);
  

  static lv_style_t field_style;

//  Object field = Object::create(container)
//		  .setPosition(0, 0)
//		  .setSize(screenHeight, screenHeight);
//		  .setStyle(&field_style);


  static lv_style_t tile_style;


 
  Object mainScreen = Object::create(container).setPosition(screenHeight, 0)
		  .setSize(screenWidth-screenHeight, screenHeight);
  
  Label x = Label::create(container).setPosition(0, 0 + 120);
  Label y = Label::create(container).setPosition(0, 40 + 120);
  Label t = Label::create(container).setPosition(0, 80 + 120);
  Label mainContent = Label::create(mainScreen).setText(" This UI was made using\n LVGLBuilder, a\n collaboration between\n teams 4911A, 7842F and 914M.\n The library aims to ease\n the creation of UIs for\n programming beginners\n and veterans alike. The\n library is currently in\n early development.\0"); 

//Object sidebar = Object::create(container).setPosition(screenWidth - 80, 0).setSize(80, screenHeight);

//  for(int i = 0; i < 4; i++){
//		  Button::create(sidebar).setPosition(0, i * 60).setSize(80, 60);
//  }

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  lv_style_copy(&container_style, &lv_style_plain);
  container_style.body.main_color = LV_COLOR_RED;
  container_style.body.grad_color = LV_COLOR_RED;
  lv_style_copy(&field_style, &lv_style_plain);
  field_style.body.main_color = LV_COLOR_WHITE;
  field_style.body.grad_color = LV_COLOR_WHITE;
  lv_style_copy(&tile_style, &lv_style_plain);
  tile_style.body.main_color = LV_COLOR_GRAY;
  tile_style.body.grad_color = LV_COLOR_GRAY;
  tile_style.body.border.color = LV_COLOR_WHITE;
  tile_style.body.border.width = 2;
  container.setStyle(&container_style);
//  for(int i = 0; i < 6; i++){
//		  for(int j = 0; j < 6; j++){
//				  Object::create(field)
//						  .setPosition(i * 40, j * 40)
//						  .setSize(40, 40)
//						  .setStyle(&tile_style);
//		  }
//  }
		switch(auton){
				case ONE_CUBE_RIGHT:		
						profileFollower.generatePath({
								okapi::PathPoint{0_ft, 0_ft, 0_deg},
								okapi::PathPoint{20_in, -2_in, 0_deg}},
								"PushCube"
						);
						break;
				case RED_RIGHT_ONE_CUBE_AND_LINE:	
						profileFollower.generatePath({
								okapi::PathPoint{0_ft, 0_ft, 0_deg},
								okapi::PathPoint{18_in, 0_ft, 0_deg}},
								"PushCube"
						);	
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{10_in, 0_in, 0_deg}},
								"LineUp"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{30_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{20_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						break;
				case RED_RIGHT_LINE_AND_PLACE:
						
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{34_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{22_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{6_in, 0_in, 0_deg}},
								"DriveIntoZone"
						);
						break;
				case BLUE_LEFT_ONE_CUBE_AND_LINE:

						profileFollower.generatePath({
								okapi::PathPoint{0_ft, 0_ft, 0_deg},
								okapi::PathPoint{18_in, 0_ft, 0_deg}},
								"PushCube"
						);	
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{9_in, 0_in, 0_deg}},
								"LineUp"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{30_in, 0_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{22_in, 0_in, 0_deg}},
								"DriveAwayFromLine"
						);
						break;
				case BLUE_LEFT_LINE_AND_PLACE:

						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{34_in, -4_in, 0_deg}},
								"DriveTowardsLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{24_in, -1_in, 0_deg}},
								"DriveAwayFromLine"
						);
						profileFollower.generatePath({
								okapi::PathPoint{0_in, 0_ft, 0_deg},
								okapi::PathPoint{6_in, 0_in, 0_deg}},
								"DriveIntoZone"
						);
						break;
				default:
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
		//stacker1.moveVoltage(127);
		//stacker2.moveVoltage(127);
		double m = 0.6;	
		switch(auton){
				case ONE_CUBE_RIGHT:
						topLeft.moveRelative(600, 127 * m);
						topRight.moveRelative(-600, 127 * m);
						bottomLeft.moveRelative(-600, 127 * m);
						bottomRight.moveRelative(600, 127 * m);
						pros::delay(2000);
						topLeft.moveRelative(-300, 127);
						topRight.moveRelative(300, 127);
						bottomLeft.moveRelative(300, 127);
						bottomRight.moveRelative(-300, 127);
						stacker1.moveVoltage(127 * 0.8);

						stacker2.moveVoltage(127 * 0.8);
						pros::delay(3000);
						stacker1.moveVoltage(0);
						stacker2.moveVoltage(0);
						break;
				case ONE_CUBE_LEFT:
						topLeft.moveRelative(-900, 127);
						topRight.moveRelative(900, 127);
						bottomLeft.moveRelative(900, 127);
						bottomRight.moveRelative(-900, 127);
						pros::delay(2000);
						topLeft.moveRelative(900, 127);
						topRight.moveRelative(-900, 127);
						bottomLeft.moveRelative(-900, 127);
						bottomRight.moveRelative(900, 127);
						stacker1.moveVoltage(127 * 0.8);

						stacker2.moveVoltage(127 * 0.8);
						pros::delay(3000);
						stacker1.moveVoltage(0);
						stacker2.moveVoltage(0);
						break;
				case RED_RIGHT_ONE_CUBE_AND_LINE:
						stacker1.moveVoltage(127 * 0.8);
						stacker2.moveVoltage(127 * 0.8);
						profileFollower.setTarget("PushCube", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("PushCube");
						profileFollower.setTarget("LineUp", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("LineUp");
						stacker1.moveVoltage(-127 * 0.8);
						stacker2.moveVoltage(-127 * 0.8);
						drivetrain.turnAngle(std::sqrt(2) * (115_deg));
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						stacker1.moveVoltage(-127 * 0.0);
						stacker2.moveVoltage(-127 * 0.0);
						drivetrain.turnAngle(std::sqrt(2) * (170_deg));
						break;
				case RED_RIGHT_LINE_AND_PLACE:
						stacker1.moveVoltage(127);
						stacker2.moveVoltage(127);
						pros::delay(1500);
						stacker1.moveVoltage(-127 * 0.8);
						stacker2.moveVoltage(-127 * 0.8);
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						drivetrain.turnAngle(std::sqrt(2) * (200_deg));
						profileFollower.setTarget("DriveIntoZone", false);
						profileFollower.waitUntilSettled();
					
						tilter.setMiddle();
						pros::delay(2000);

						tilter.setUp();
						pros::delay(500);

						stacker1.moveVoltage(127 * 0.4);
						stacker2.moveVoltage(127 * 0.4);
						
						profileFollower.setTarget("DriveIntoZone", true);
						profileFollower.waitUntilSettled();
						tilter.setDown();
						break;
				case BLUE_LEFT_ONE_CUBE_AND_LINE:

						stacker1.moveVoltage(127 * 0.8);
						stacker2.moveVoltage(127 * 0.8);
						profileFollower.setTarget("PushCube", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("PushCube");
						profileFollower.setTarget("LineUp", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("LineUp");
						stacker1.moveVoltage(-127 * 0.8);
						stacker2.moveVoltage(-127 * 0.8);
						drivetrain.turnAngle(std::sqrt(2) * (-115_deg));
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						stacker1.moveVoltage(-127 * 0.0);
						stacker2.moveVoltage(-127 * 0.0);
						drivetrain.turnAngle(std::sqrt(2) * (-170_deg));
						break;
				case BLUE_LEFT_LINE_AND_PLACE:

						stacker1.moveVoltage(127);
						stacker2.moveVoltage(127);
						pros::delay(1500);
						stacker1.moveVoltage(-127 * 0.8);
						stacker2.moveVoltage(-127 * 0.8);
						profileFollower.setTarget("DriveTowardsLine", false);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveTowardsLine");
						profileFollower.setTarget("DriveAwayFromLine", true);
						profileFollower.waitUntilSettled();
						profileFollower.removePath("DriveAwayFromLine");
						drivetrain.turnAngle(-std::sqrt(2) * (180_deg));
						profileFollower.setTarget("DriveIntoZone", false);
						profileFollower.waitUntilSettled();
						stacker1.moveVoltage(127 * 0.5);
						stacker2.moveVoltage(127 * 0.5);
						pros::delay(500);
						stacker1.moveVoltage(127 * 0);
						stacker2.moveVoltage(127 * 0);
						tilter.setMiddle();
						pros::delay(1500);
						tilter.setUp();
						pros::delay(1000);

						stacker1.moveVoltage(127 * 0.6);
						stacker2.moveVoltage(127 * 0.6);
						pros::delay(400);	
						profileFollower.setTarget("DriveIntoZone", true);
						profileFollower.waitUntilSettled();
						tilter.setDown();
						break;
				case BLUE_GIRL_POWER:
						drivetrain.moveDistance(20_in);
						pros::delay(1000);
						drivetrain.turnAngle(120_deg);
						pros::delay(1000);
						drivetrain.moveDistance(30_in);
						pros::delay(1000);
						//move
						//drivetrain.moveDistance(20_in);
						drivetrain.moveDistance(-10_in);
						pros::delay(1000);
						//stacker1.moveVoltage(127);
						//stacker2.moveVoltage(127);
						pros::delay(3000);
						stacker1.moveVoltage(0);
						stacker2.moveVoltage(0);
				case RED_GIRL_POWER:
						topLeft.moveRelative(900, 127* 0.8);
						topRight.moveRelative(-900, 127);
						bottomLeft.moveRelative(-900, 127);
						bottomRight.moveRelative(900, 127 * 0.8);
						pros::delay(2000);
						topLeft.moveRelative(-900, 127);
						topRight.moveRelative(900, 127);
						bottomLeft.moveRelative(900, 127);
						bottomRight.moveRelative(-900, 127);
						pros::delay(2000);
						drivetrain.moveDistance(-23_in);
						drivetrain.turnAngle(-160_deg);
						drivetrain.moveDistance(-30_in);
						//move
						//drivetrain.moveDistance(20_in);
						drivetrain.moveDistance(10_in);
						pros::delay(1000);
						//stacker1.moveVoltage(127);
						//stacker2.moveVoltage(127);
						pros::delay(3000);
						stacker1.moveVoltage(0);
						stacker2.moveVoltage(0);
 				default:
						break;
		}

		//if(isLine){

				//stacker1.moveVoltage(-127 * 0.6);
				//stacker2.moveVoltage(-127 * 0.6);
				
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
  stacker1.moveVoltage(stacker.getOutput());
  stacker2.moveVoltage(stacker.getOutput());
  drive.in();
  drive.out();
  tracker.in();
  tracker.out();
  if(left.update(master.get_digital(DIGITAL_L2))){
    tilter.shiftDown();
  }
  tilter.in();
  tilter.out();
  if(right.update(master.get_digital(DIGITAL_L1))){
		  tilter.shiftUp();
  }else if(master.get_digital(DIGITAL_DOWN)){
		tilter.adjustThrottle(-127 * 0.5);
  }else{
		tilter.adjustThrottle(0);
  }
  if(master.get_digital(DIGITAL_X)){
tracker.resetRotation();
  }
}

void updateScreen(){
		char xbuffer [50];
		char ybuffer [50];
		char tbuffer [50];
		std::sprintf(xbuffer, "X: %f", tracker.getGlobalPosition().getX());
		std::sprintf(ybuffer, "Y: %f", tracker.getGlobalPosition().getY());
		std::sprintf(tbuffer, "Theta: %f", tracker.getTheta());
		x.setText(xbuffer);
		y.setText(ybuffer);
		t.setText(tbuffer);
		/*  UIHelper::updateDisplay(
    tracker.getLEncValue(),
    tracker.getREncValue(),
    tracker.getBEncValue(),
    tracker.getGlobalPosition().getX(),
    tracker.getGlobalPosition().getY(),
    tracker.getTheta()
  );*/
}

void opcontrol() {
	while (true) {
    runSubsystems();
    updateScreen();
		pros::delay(20);
	}
}
