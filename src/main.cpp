#include "main.h"

#include "lib7842/api.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter.hpp"
using namespace lib7842;





  Controller controller(ControllerId::master);

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

  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({2.75_in, 12.9473263_in, 0.00_in}, 360));

  Screen scr(lv_scr_act(), LV_COLOR_ORANGE);

  auto odomController = std::make_shared<OdomXController>(
    model, odom,
    //Distance PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.015, 0.0002, 0.0002, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    //Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.03, 0.00, 0.0003, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    //Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.02, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
    //Strafe PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.015, 0.0002, 0.0002, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    2_in);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::delay(200);
  odom->startTask("Odometry");
  scr.makePage<OdomDebug>().attachOdom(odom);
  scr.startTask("Screen");
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


enum Auton{
		BLUE_SMALL_ZONE_ONE_CUBE,
		BLUE_BIG_ZONE_ONE_CUBE,
		RED_SMALL_ZONE_ONE_CUBE,
		RED_BIG_ZONE_ONE_CUBE,
		BLUE_SMALL_ZONE_5STACK,
		RED_SMALL_ZONE_5STACK,
		BLUE_BIG_ZONE_3STACK,
		RED_BIG_ZONE_3STACK,
		RED_BIG_ZONE_PUSH,
		BLUE_BIG_ZONE_PUSH
};

Auton currentAuton = RED_SMALL_ZONE_5STACK;

const QLength botWidth = 18_in;
const QLength botLength = 18_in;
const QLength tileLength = 2_ft;
const QLength cubeWidth = 5.5_in;
const QLength fieldWidth = 6 * tileLength;
const QLength towerBaseWidth = 7.8_in;
const QLength zoneWidth = 10_in;
const QLength bigZoneLength = 15.5_in;
const QLength barrierWidth = 2_in;
const Vector redInnerProtectedCube = {tileLength - cubeWidth/2, tileLength + cubeWidth/2};
const Vector redOuterProtectedCube = {2 * tileLength - cubeWidth/2, tileLength + cubeWidth/2};
const Vector red4Stack = {2 * tileLength - cubeWidth/2, 2 * tileLength + cubeWidth/2};
const Vector red3StackBack = {3 * tileLength - cubeWidth/2, 2 * tileLength + cubeWidth/2};
const Vector red2StackBack = {4 * tileLength - cubeWidth/2, 2 * tileLength + cubeWidth/2};
const Vector redLineBack = {5 * tileLength - cubeWidth/2, 2 * tileLength + cubeWidth/2};
const Vector red3StackFront = {3 * tileLength - cubeWidth/2, 2 * tileLength - cubeWidth/2};
const Vector red2StackFront = {4 * tileLength - cubeWidth/2, 2 * tileLength - 1.5 * cubeWidth/2};
const Vector redLineFront = {5 * tileLength - cubeWidth/2, 2 * tileLength  - 2.5 * cubeWidth/2};
const Vector redSmallZoneCorner = {fieldWidth - zoneWidth - barrierWidth, zoneWidth + barrierWidth};
const Vector leftCorner = {0_in, 0_in};
const Vector rightCorner = {fieldWidth, 0_in};

void autonomous() {
		switch(currentAuton){
				case BLUE_SMALL_ZONE_ONE_CUBE:{
						odom->setState(State(
								tileLength + botWidth/2, //x
								botLength/2,//y
								0_deg));//theta	
						break;
				}
				case BLUE_BIG_ZONE_ONE_CUBE:{
						odom->setState(State(
								6 * tileLength - botWidth/2, 
								botLength/2, 
								0_deg));	
						break;
				}
				case RED_SMALL_ZONE_ONE_CUBE:{
						odom->setState(State(
								5 * tileLength - botWidth/2, 
								botLength/2, 
								0_deg));	
						break;
				}
				case RED_BIG_ZONE_ONE_CUBE:{
						odom->setState(State(
								tileLength + botWidth/2, 
								botLength/2, 
								0_deg));	
						break;
				}
				case BLUE_SMALL_ZONE_5STACK:{
						odom->setState(State(
								tileLength + cubeWidth/2, 
								botLength/2, 
								0_deg));	
						break;
				}
				case RED_SMALL_ZONE_5STACK:{
						odom->setState(State(
								5 * tileLength - cubeWidth/2,//x 
								botLength/2, 
								0_deg));	

						odomController->strafeToPoint(
						redLineFront - Vector(0_in, botLength/2 + 1_in), OdomController::makeAngleCalculator(redLineBack), 1,
						OdomController::defaultDriveAngleSettler);
						//start intaking
						
						odomController->strafeToPoint(
						redLineBack, OdomController::makeAngleCalculator(redLineBack), 1,
						OdomController::defaultDriveAngleSettler);
						//stop intaking

						odomController->strafeToPoint(
						redSmallZoneCorner + Vector(-3_in, 3_in), OdomController::makeAngleCalculator(rightCorner), 1,
						OdomController::defaultDriveAngleSettler);
						//place stack

						break;
				}
				case BLUE_BIG_ZONE_3STACK:{
						odom->setState(State(4 * tileLength + cubeWidth/2, botLength/2, 0_deg));	
						break;
				}
				case RED_BIG_ZONE_3STACK:{
						odom->setState(State(2 * tileLength - cubeWidth/2, botLength/2, 0_deg));	
						break;
				}
				default:
						break;
		}

      odomController->strafeToPoint(
        {0_ft, 0_ft}, OdomController::makeAngleCalculator({0_ft, 3_ft}), 1,
        OdomController::defaultDriveAngleSettler);
}

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
Stacker stacker = *Stacker::getInstance();
Tilter tilter = *Tilter::getInstance();
LatchedBoolean left = LatchedBoolean();
LatchedBoolean right = LatchedBoolean();

auto stacker1 = okapi::Motor(6);
auto stacker2 = okapi::Motor(-5);

void opcontrol() {
  while (true) {
    model->xArcade(
      controller.getAnalog(ControllerAnalog::rightX), controller.getAnalog(ControllerAnalog::rightY),
      controller.getAnalog(ControllerAnalog::leftX));

		if(controller.getDigital(ControllerDigital::left)){
				stacker.slowOuttake();
		}else if(controller.getDigital(ControllerDigital::R1)){
				stacker.outtake();
		}else if(controller.getDigital(ControllerDigital::R2)){
				stacker.intake();
		}else{
				stacker.stop();
		}

		stacker.in();
		stacker.out();
		
		tilter.in();
		tilter.out();

		if(left.update(controller.getDigital(ControllerDigital::L2))){
				tilter.shiftDown();
		}
		if(right.update(controller.getDigital(ControllerDigital::L1))){
				tilter.shiftUp();
		}else if(controller.getDigital(ControllerDigital::down)){
				tilter.adjustThrottle(-127 * 0.3);
		}else{
				tilter.adjustThrottle(0);
		}

		stacker1.moveVoltage(stacker.getOutput());
		stacker2.moveVoltage(stacker.getOutput());

    pros::delay(10);
  }
}
