#include "main.h"

#include "lib7842/api.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter.hpp"
#include "util/util.hpp"
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
	//P - 0.0032 -> 0.0128 started to oscillate
    std::make_unique<IterativePosPIDController>(
      0.008, 0.000, 0.0009, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    //Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.03, 0.00, 0.0003, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    //Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.02, 0, 0.0, 0, TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)),
    //Strafe PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.5, 0.000, 0.1, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
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
		pros::delay(1500);
		rollerStop();
   }
   void placeStack(){
		tilter.setMiddle();
		pros::delay(2000);
		tilter.setUp();
		pros::delay(1300);
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
		SMALL_ZONE_6STACK,
		SMALL_ZONE_7STACK,
		BIG_ZONE_3STACK,
		BIG_ZONE_PUSH,
		BIG_ZONE_KNOCKER,
		TEST
};

enum Color{
		RED,
		BLUE
};

Color alliance = RED;
Auton currentAuton = SMALL_ZONE_5STACK;

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
		tilter.setDown();
		switch(currentAuton){
				case TEST:{
						//deployTray();
						odomController->strafeToPoint(
										{0_in, 48_in},
						OdomController::makeAngleCalculator(90_deg), 1,
						OdomController::makeSettler(0.5_in, 0.5_deg));

						break;
				}
				case SMALL_ZONE_ONE_CUBE:{
						switch(alliance){
								case RED:{
										odomController->strafeToPoint(
										{cubeWidth + barrierWidth + 7_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{0_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
								case BLUE:{
										odomController->strafeToPoint(
										{-(cubeWidth + barrierWidth + 5_in), 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{0_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
						}
						deployTray();
						break;
				}
				case BIG_ZONE_ONE_CUBE:{
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{cubeWidth + barrierWidth + 7_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{0_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-(cubeWidth + barrierWidth + 7_in), 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{0_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
						}
						deployTray();
						break;
				}
				case SMALL_ZONE_5STACK:{

						odomController->strafeToPoint(
						{0_in, 10_in},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(2_in, 5_deg));

						rollerOuttake();

						odomController->strafeToPoint(
						{0_in, 0_in},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(2_in, 5_deg));

						pros::delay(1000);

						rollerIntake();
						
						model->setMaxVoltage(4000);

						odomController->strafeToPoint(
						{0_in, 1.5_tl},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(4_in, 5_deg));

						model->setMaxVoltage(12000);

						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{0_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(-135_deg), 3,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{-4_in, 1_tl/2},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{0_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(135_deg), 3,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{6_in, 1_tl/2 + 1_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
						}

//						rollerOuttake();
//						pros::delay(100);
//						rollerStop();
						placeStack();
							
//						rollerOuttake();
//						pros::delay(50);
//						rollerStop();
						model->setMaxVoltage(4000);
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{-1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
						}
						break;
				}
				case SMALL_ZONE_6STACK:{
						rollerOuttake();
				    switch(alliance){
								case BLUE:{
												odomController->strafeToPoint(
												{-3_in, 3_in},
												OdomController::makeAngleCalculator(0_deg), 1,
												OdomController::makeSettler(2_in, 5_deg));
												break;
								}
								case RED:{
												odomController->strafeToPoint(
												{3_in, 3_in},
												OdomController::makeAngleCalculator(0_deg), 1,
												OdomController::makeSettler(2_in, 5_deg));
												break;
								}
				    }
						pros::delay(1000);

						rollerIntake();
						
						model->setMaxVoltage(4000);

						odomController->strafeToPoint(
						{0_in, 1.5_tl},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(4_in, 5_deg));

						model->setMaxVoltage(8000);

						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{3_in, 2.1_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										model->setMaxVoltage(12000);

										odomController->strafeToPoint(
										{3_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(-135_deg), 2,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{-4_in, 1_tl/2},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-3_in, 2.1_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										
										model->setMaxVoltage(12000);

										odomController->strafeToPoint(
										{-3_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(135_deg), 2,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{6_in, 1_tl/2 + 1_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
						}

//						rollerOuttake();
//						pros::delay(100);
//						rollerStop();
						placeStack();
							
//						rollerOuttake();
//						pros::delay(50);
//						rollerStop();
						model->setMaxVoltage(4000);
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{-1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
						}
						break;
				}
				case SMALL_ZONE_7STACK:{
						rollerOuttake();
				    switch(alliance){
								case BLUE:{
												odomController->strafeToPoint(
												{-3_in, 3_in},
												OdomController::makeAngleCalculator(0_deg), 1,
												OdomController::makeSettler(2_in, 5_deg));
												break;
								}
								case RED:{
												odomController->strafeToPoint(
												{3_in, 3_in},
												OdomController::makeAngleCalculator(0_deg), 1,
												OdomController::makeSettler(2_in, 5_deg));
												break;
								}
				    }
						pros::delay(1000);

						rollerIntake();
						
						model->setMaxVoltage(6000);

						odomController->strafeToPoint(
						{0_in, 1.5_tl},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(6_in, 5_deg));

						model->setMaxVoltage(8000);

						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{-2_in, 2.1_tl},
										OdomController::makeAngleCalculator(30_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{3_in, 1.7_tl},
										OdomController::makeAngleCalculator(-45_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{8_in, 1.9_tl},
										OdomController::makeAngleCalculator(-45_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										model->setMaxVoltage(12000);

										odomController->strafeToPoint(
										{3_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(-135_deg), 2,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{-4_in, 1_tl/2},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{2_in, 2.1_tl},
										OdomController::makeAngleCalculator(-30_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{-3_in, 1.7_tl},
										OdomController::makeAngleCalculator(-45_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										odomController->strafeToPoint(
										{-8_in, 1.9_tl},
										OdomController::makeAngleCalculator(-45_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										
										model->setMaxVoltage(12000);

										odomController->strafeToPoint(
										{-3_in, 1_tl - 6_in},
										OdomController::makeAngleCalculator(135_deg), 2,
										OdomController::makeSettler(3_in, 10_deg));

										rollerStop();

										odomController->strafeToPoint(
										{6_in, 1_tl/2 + 1_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 5_deg));
										break;
								}
						}

//						rollerOuttake();
//						pros::delay(100);
//						rollerStop();
						placeStack();
							
//						rollerOuttake();
//						pros::delay(50);
//						rollerStop();
						model->setMaxVoltage(4000);
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(-135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-6_in, 1_tl + 5_in},
										OdomController::makeAngleCalculator(135_deg), 1,
										OdomController::makeSettler(3_in, 10_deg));
										model->setMaxVoltage(12000);

										tilter.setDown();

										odomController->strafeToPoint(
										{-1_tl, 0.5_tl},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));
										break;
								}
						}
						break;
				}
				case BIG_ZONE_3STACK:{
						start(outerProtectedCube.x);
						break;
				}
				case BIG_ZONE_PUSH:{
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{cubeWidth + barrierWidth + 11_in, 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-(cubeWidth + barrierWidth + 11_in), 0_in},
										OdomController::makeAngleCalculator(0_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										break;
								}
						}
						odomController->strafeToPoint(
						{0_in, 0_in},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(4_in, 5_deg));

						odomController->strafeToPoint(
						{0_in, 1.4_tl},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(4_in, 5_deg));

						model->setMaxVoltage(8000);
						switch(alliance){
								case BLUE:{
										odomController->strafeToPoint(
										{cubeWidth + barrierWidth + 9_in, bigZoneLength + barrierWidth - 9_in },
										OdomController::makeAngleCalculator(55_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										break;
								}
								case RED:{
										odomController->strafeToPoint(
										{-(cubeWidth + barrierWidth + 9_in), bigZoneLength + barrierWidth - 9_in},
										OdomController::makeAngleCalculator(-55_deg), 1,
										OdomController::makeSettler(4_in, 5_deg));

										break;
								}
						}
						model->setMaxVoltage(12000);
						odomController->strafeToPoint( {0_in, 0_in},
						OdomController::makeAngleCalculator(0_deg), 1,
						OdomController::makeSettler(4_in, 5_deg));
						//deployTray();
						break;
				}
				default:
						break;
		}
}

Stacker stacker = *Stacker::getInstance();
LatchedBoolean left = LatchedBoolean();
LatchedBoolean right = LatchedBoolean();
LatchedBoolean offsetPressForward = LatchedBoolean();
LatchedBoolean offsetPressBackward = LatchedBoolean();

void opcontrol() {
		model->setMaxVoltage(12000);
  while (true) {
		  double turn = Util::map(Util::curveJoystick(
						  false, 
						  Util::map(controller.getAnalog(ControllerAnalog::leftX), -1, 1, -127, 127), 
						  15), -127, 127, -1, 1); 
		  double xPower = Util::map(Util::curveJoystick(
						  false, 
						  Util::map(controller.getAnalog(ControllerAnalog::rightX), -1, 1, -127, 127), 
						  10), -127, 127, -1, 1); 
		  double yPower = Util::map(Util::curveJoystick(
						  false, 
						  Util::map(controller.getAnalog(ControllerAnalog::rightY), -1, 1, -127, 127), 
						  10), -127, 127, -1, 1); 
    model->xArcade(
      xPower,
      yPower,
      turn);

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
				tilter.adjustThrottle(-127 * 0.5);
		}else{
				tilter.adjustThrottle(0);
		}

		if(offsetPressForward.update(controller.getDigital(ControllerDigital::B))){
				tilter.offsetForward();
		}
		if(offsetPressBackward.update(controller.getDigital(ControllerDigital::X))){
				tilter.offsetBackward();
		}

		stackerMotor1->moveVoltage(stacker.getOutput());
		stackerMotor2->moveVoltage(stacker.getOutput());

    pros::delay(10);
  }
}
