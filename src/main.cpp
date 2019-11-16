#include "main.h"

#include "lib7842/api.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter.hpp"
using namespace lib7842;
using namespace okapi;

  Controller controller(ControllerId::master);

  auto model = std::make_shared<ThreeEncoderXDriveModel>(
    // motors
    std::make_shared<Motor>(4), //topLeft
    std::make_shared<Motor>(-3), //topRight
    std::make_shared<Motor>(-1), //bottomRight
    std::make_shared<Motor>(2), //bottomLeft
    // sensors
    std::make_shared<ADIEncoder>(2, 1, false), // left
    std::make_shared<ADIEncoder>(3, 4), // right
    std::make_shared<ADIEncoder>(5, 6), // middle
    // limits
    200, 12000);//maxVel, maxVoltage

  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({2.75_in, 14_in, 0.00_in}, 360));

  Screen scr(lv_scr_act(), LV_COLOR_RED);

  auto odomController = std::make_shared<OdomXController>(
    model, odom,
    //Distance PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.01, 0.000, 0.00005, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    //Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.03, 0.00, 0.000, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    //Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.02, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
    //Strafe PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.010, 0.000, 0.000, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    2_in);

   Tilter tilter = *Tilter::getInstance();
   auto stackerMotor1 = std::make_shared<Motor>(6);
   auto stackerMotor2 = std::make_shared<Motor>(-5);

   void rollerOuttake(){
		stackerMotor1->moveVoltage(12000);
		stackerMotor2->moveVoltage(12000);
   }
   void rollerIntake(){
		stackerMotor1->moveVoltage(-12000);
		stackerMotor2->moveVoltage(-12000);
   }
   void rollerStop(){
		stackerMotor1->moveVoltage(0);
		stackerMotor2->moveVoltage(0);
   }
   void deployTray(){
		rollerOuttake();
		pros::delay(3000);
		rollerStop();
   }
   void placeStack(){
		tilter.setMiddle();
		pros::delay(2000);
		tilter.setUp();
   }

//tune these
const QLength botWidth = 17.5_in;
const QLength botLength = 17.5_in;

const QLength cubeWidth = 5.5_in;
const QLength fieldWidth = 6 * tile;
const QLength towerBaseWidth = 7.8_in;
const QLength zoneWidth = 10_in;
const QLength bigZoneLength = 15.5_in;
const QLength barrierWidth = 2_in;
//field positions
//constants
Vector leftCorner = {0_in, 0_in};
Vector rightCorner = {fieldWidth, 0_in};
Vector closeTower = {3 * tile, tile};
Vector bottomCloseTowerCube = closeTower - Vector(0_in, towerBaseWidth + cubeWidth/2);
Vector leftCloseTowerCube = closeTower - Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector rightCloseTowerCube = closeTower + Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector leftTower = {1.5 * tile, 3 * tile};
Vector bottomLeftTowerCube = leftTower - Vector(0_in, towerBaseWidth + cubeWidth/2);
Vector leftLeftTowerCube = leftTower - Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector rightLeftTowerCube = leftTower + Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector middleTower = {3 * tile, 3 * tile};
Vector bottomMiddleTowerCube = middleTower - Vector(0_in, towerBaseWidth + cubeWidth/2);
Vector leftMiddleTowerCube = middleTower - Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector rightMiddleTowerCube = middleTower + Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector rightTower = {4.5 * tile, 3 * tile};
Vector bottomRightTowerCube = rightTower - Vector(0_in, towerBaseWidth + cubeWidth/2);
Vector leftRightTowerCube = rightTower - Vector(towerBaseWidth + cubeWidth/2, 0_in);
Vector rightRightTowerCube = rightTower + Vector(towerBaseWidth + cubeWidth/2, 0_in);

//opposite side of field

//Vector topLeftTowerCube = leftTower + Vector(0_in, towerBaseWidth + cubeWidth/2);
//Vector topMiddleTowerCube = middleTower + Vector(0_in, towerBaseWidth + cubeWidth/2);
//Vector topRightTowerCube = rightTower + Vector(0_in, towerBaseWidth + cubeWidth/2);

//vary by alliance color
Vector innerProtectedCube;
Vector outerProtectedCube;
Vector stack4;
Vector stack3Back;
Vector stack2Back;
Vector lineBack;
Vector stack3Front;
Vector stack2Front;
Vector lineFront;
Vector smallBarrierCorner;
Vector bigBarrierCorner;
Vector smallCorner;
Vector bigCorner;
Vector allianceTower;
Vector autonLineSingleCube;
Vector autonLineBlockMiddle;
Vector autonLineBlockInnerCorner;


enum Auton{
		SMALL_ZONE_ONE_CUBE,
		BIG_ZONE_ONE_CUBE,
		SMALL_ZONE_5STACK,
		BIG_ZONE_3STACK,
		BIG_ZONE_PUSH,
		TEST
};

enum Color{
		RED,
		BLUE
};

Color alliance = BLUE;
Auton currentAuton = TEST;

void initialize() {
  pros::delay(200);
  odom->startTask("Odometry");
  scr.makePage<OdomDebug>().attachOdom(odom);
  scr.startTask("Screen");

  switch(alliance){
		  case RED:{
				innerProtectedCube = {tile - cubeWidth/2, tile + cubeWidth/2};
				outerProtectedCube = {2 * tile - cubeWidth/2, tile + cubeWidth/2};
				stack4 = {2 * tile - cubeWidth/2, 2 * tile + cubeWidth/2};
				stack3Back = {3 * tile - cubeWidth/2, 2 * tile + cubeWidth/2};
				stack2Back = {4 * tile - cubeWidth/2, 2 * tile + cubeWidth/2};
				lineBack = {5 * tile - cubeWidth/2, 2 * tile + cubeWidth/2};
				smallBarrierCorner = {fieldWidth - zoneWidth - barrierWidth, zoneWidth + barrierWidth};
				bigBarrierCorner = {zoneWidth + barrierWidth, bigZoneLength + barrierWidth};
				allianceTower = {fieldWidth, 1.5 * tile};
				autonLineSingleCube = {cubeWidth/2, 3 * tile};
				autonLineBlockMiddle = {fieldWidth - cubeWidth, 3 * tile};
				autonLineBlockInnerCorner = {fieldWidth - 1.5 * cubeWidth, 3 * tile - cubeWidth};
				smallCorner = rightCorner;
				bigCorner = leftCorner;
				break;
		  }
		  case BLUE:{
				innerProtectedCube = {5 * tile + cubeWidth/2, tile + cubeWidth/2};
				outerProtectedCube = {4 * tile + cubeWidth/2, tile + cubeWidth/2};
				stack4 = {4 * tile + cubeWidth/2, 2 * tile + cubeWidth/2};
				stack3Back = {3 * tile + cubeWidth/2, 2 * tile + cubeWidth/2};
				stack2Back = {2 * tile + cubeWidth/2, 2 * tile + cubeWidth/2};
				lineBack = {tile + cubeWidth/2, 2 * tile + cubeWidth/2};
				smallBarrierCorner = {zoneWidth + barrierWidth, zoneWidth + barrierWidth};
				bigBarrierCorner = {fieldWidth - zoneWidth - barrierWidth, bigZoneLength + barrierWidth};
				allianceTower = {0_in, 1.5 * tile};
				autonLineSingleCube = {fieldWidth - cubeWidth/2, 3 * tile};
				autonLineBlockMiddle = {cubeWidth, 3 * tile};
				autonLineBlockInnerCorner = {1.5 * cubeWidth, 3 * tile - cubeWidth};
				smallCorner = leftCorner;
				bigCorner = rightCorner;
				break;
		  }
		  stack3Front = stack3Back - Vector(0_in, cubeWidth); 
		  stack2Front = stack2Back - Vector(0_in, 2 * cubeWidth);
		  lineFront = lineBack - Vector(0_in, 3 * cubeWidth); 
  }
}

void disabled() {}

void competition_initialize() {}



void start(QLength x){
		odom->setState(State(
		x, //x
		botLength/2,//y
		0_deg));//theta	
};

QLength negativeIfRed(QLength val){
		if(alliance == RED){
				return -val;
		}
		return val;
}

void autonomous() {
		switch(currentAuton){
				case TEST:{
						deployTray();
						odomController->strafeToPoint(
										{0_in, 30_in},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(1_in, 5_deg));
						break;
				}
				case SMALL_ZONE_ONE_CUBE:{
						start(smallBarrierCorner.x + negativeIfRed(cubeWidth + 1_in + botWidth/2));
						
						rollerOuttake();

						//drive cube into zone
						odomController->strafeToPoint(
						smallCorner + Vector(negativeIfRed(zoneWidth + botWidth/2), botLength/2),
						OdomController::makeAngleCalculator(0_deg), 0,
						OdomController::defaultDriveSettler);

						//drive to front of line
						odomController->strafeToPoint(
						lineFront - Vector(0_in, botLength/2 + 1_in), 
						OdomController::makeAngleCalculator(lineFront), 1,
						OdomController::defaultDriveAngleSettler);

						//start intaking
						rollerIntake();

						//drive to back of line
						odomController->strafeToPoint(
						lineBack, OdomController::makeAngleCalculator(lineBack), 1,
						OdomController::defaultDriveAngleSettler);
						
						//stop intaking
						rollerStop();

						//drive towards zone
						//may need to adjust angle
						odomController->strafeToPoint(
						smallBarrierCorner + Vector(negativeIfRed(6_in), 6_in), OdomController::makeAngleCalculator(smallCorner), 1,
						OdomController::defaultDriveAngleSettler);
						
						//place stack
						//placeStack();
						break;
				}
				case BIG_ZONE_ONE_CUBE:{
						start(bigBarrierCorner.x + negativeIfRed(cubeWidth + 1_in));
						break;
				}
				case SMALL_ZONE_5STACK:{
						//set starting position
						start(lineFront.x);

						//deploy tray
						rollerOuttake();

						//drive to front of line (make loose settle, it's going straight)
						odomController->strafeToPoint(
						lineFront - Vector(0_in, botLength/2 + 1_in), 
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(6_in, 5_deg));
						
						//wait for tray to deploy
						pros::delay(500);

						//start intaking
						rollerIntake();

						//drive to back of line
						odomController->strafeToPoint(
						lineBack, OdomController::makeAngleCalculator(lineBack), 1,
						OdomController::defaultDriveAngleSettler);

						//stop intaking
						rollerStop();

						//drive towards zone
						//may need to adjust angle
						odomController->strafeToPoint(
						smallBarrierCorner + Vector(negativeIfRed(3_in), 3_in), OdomController::makeAngleCalculator(smallCorner), 1,
						OdomController::defaultDriveAngleSettler);
						
						//place stack
						placeStack();

						//drive towards next line
						odomController->strafeToPoint(
						stack2Front - Vector(0_in, botLength/2 + 1_in), 
						OdomController::makeAngleCalculator(stack2Front), 1,
						OdomController::makeSettler(6_in, 5_deg));

						break;
				}
				case BIG_ZONE_3STACK:{
						start(outerProtectedCube.x);
						break;
				}
				case BIG_ZONE_PUSH:{
						start(bigBarrierCorner.x + negativeIfRed(cubeWidth + 1_in));
						break;
				}
				default:
						break;
		}
}

Stacker stacker = *Stacker::getInstance();
LatchedBoolean left = LatchedBoolean();
LatchedBoolean right = LatchedBoolean();


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
				tilter.adjustThrottle(-127 * 0.2);
		}else{
				tilter.adjustThrottle(0);
		}

		stackerMotor1->moveVoltage(stacker.getOutput());
		stackerMotor2->moveVoltage(stacker.getOutput());

    pros::delay(10);
  }
}
