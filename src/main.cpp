#include "main.h"
#include "lib7842/api.hpp"
#include "subsystems/stacker.hpp"
#include "subsystems/tilter.hpp"
#include "subsystems/arm.hpp"
#include "util/util.hpp"
#include "ui/autonselector.hpp"
using namespace lib7842;
using namespace lib7842::units;
using namespace okapi;

Controller controller{ControllerId::master};

//This is the model of the robot chassis
//It keeps track of the motors, encoder wheels, max Velocity (in RPM), and max Voltage
auto model {std::make_shared<ThreeEncoderXDriveModel>(
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
    	200,//max Velocity
	12000//max Voltage
)};

//keeps track of position and orientation based on bot measurements
auto odom {std::make_shared<CustomOdometry>(
	model,
	ChassisScales({
		2.792_in,//Encoder wheel diameter
		14.0006164_in,//Drive base width, assumes encoder wheels are the same distance from the center
		6.50_in//Middle wheel distance, which doesn't matter to the math
	}, 360),//Encoder ticks per rotation
	TimeUtilFactory().create()
)};

GUI::Screen scr{lv_scr_act(), LV_COLOR_RED};

auto odomController {std::make_shared<OdomXController>(
	model, odom,
    	//Distance PID - To mm
	//Used for strafing and driving
    	std::make_unique<IterativePosPIDController>(
		0.008, 0.000, 0.0009, 0, 
		TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)
	),
    	//Turn PID - To Degree
	//Used for turning
    	std::make_unique<IterativePosPIDController>(
      		0.03, 0.00, 0.0003, 0,
		TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)
	),
    	//Angle PID - To Degree
	//Used for turning
    	std::make_unique<IterativePosPIDController>(
		0.02, 0, 0.0, 0,
		TimeUtilFactory::withSettledUtilParams(4, 2, 100_ms)
	),
	TimeUtilFactory().create()
)};

Tilter tilter {*Tilter::getInstance()};

//replace with a motor group
auto stackerMotor1 {std::make_shared<Motor>(6)};
auto stackerMotor2 {std::make_shared<Motor>(-5)};

//Sets the rollers to full speed outward
void rollerOuttake(){
     	stackerMotor1->moveVoltage(12000);
     	stackerMotor2->moveVoltage(12000);
}

//Sets the rollers to a specific speed
//accepts t values in [-1, 1].
//negative values intake, positive values outtake
void rollerOuttake(double t){
     	stackerMotor1->moveVoltage(12000 * t);
     	stackerMotor2->moveVoltage(12000 * t);
}

//Sets the motors to full speed inward
void rollerIntake(){
     	stackerMotor1->moveVoltage(-12000);
     	stackerMotor2->moveVoltage(-12000);
}

//Stops the rollers
void rollerStop(){
     	stackerMotor1->moveVoltage(0);
     	stackerMotor2->moveVoltage(0);
}

//Macro for deploying the tray
//It prevents code progression for 1.5 seconds, try using a task maybe?
void deployTray(){
     	rollerOuttake();
     	pros::delay(1500);
     	rollerStop();
}

//Macro for placing a stack
//Prevents code progression for 3.6 seconds
void placeStack(){
	rollerStop();
     	tilter.setUp();
}

//Constant distances used for autonomous plotting
const QLength botWidth {17.5_in};
const QLength botLength {17.5_in};

const QLength cubeWidth {5.5_in};
const QLength fieldWidth {6_tile};
const QLength towerBaseWidth {7.8_in};
const QLength zoneWidth {10_in};
const QLength bigZoneLength {15.5_in};
const QLength barrierWidth {2_in};



//These are used to determine which autonomous is run
//Currently are updated based on the autonomous selector
AutonSelector::Color alliance {AutonSelector::getColor()};
AutonSelector::Auton currentAuton{AutonSelector::getAuton()};

void initialize() {
	pros::delay(200);

	//Start UI
	odom->startTask("Odometry");
	scr.makePage<AutonSelector>();
	scr.makePage<GUI::Odom>().attachOdom(odom);
	scr.startTask("Screen");
}

void disabled() {}

void competition_initialize() {}

//Wraps the odomController calls for easier autonomous programming
//Takes a target position and orientation
void driveToPoint(
		Vector target, 
		QAngle targetAngle=0_deg, 
		double turnPriority=1, 
		QLength settleDistance=4_in, 
		QAngle settleAngle=5_deg){
	odomController->strafeToPoint(
	target,
	OdomController::makeAngleCalculator(targetAngle), turnPriority,
	OdomController::makeSettler(settleDistance, settleAngle));
}

//Same as above, but faces a point during the motion instead of a set orientation
void driveToPoint(
		Vector target, 
		Vector pointToFace, 
		double turnPriority=1, 
		QLength settleDistance=4_in, 
		QAngle settleAngle=5_deg){

	odomController->strafeToPoint(
	target,
	OdomController::makeAngleCalculator(pointToFace), turnPriority,
	OdomController::makeSettler(settleDistance, settleAngle));
}

void autonomous() {
	odom->reset();
	model->setMaxVoltage(12000);
	//Get the chosen alliance and auton
	alliance = AutonSelector::getColor();
	currentAuton = AutonSelector::getAuton(); 

	tilter.setDown();
	switch(currentAuton){
		case AutonSelector::Auton::TEST:{
			tilter.setUp();
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_ONE_CUBE:{
			switch(alliance){
				case AutonSelector::Color::RED:{
					//push cube into zone
					driveToPoint({cubeWidth + barrierWidth + 7_in, 0_in});
					//drive away from zone
					driveToPoint({0_in, 0_in});
					break;
				}
				case AutonSelector::Color::BLUE:{
					//push cube into zone
					driveToPoint({-(cubeWidth + barrierWidth + 7_in), 0_in});
					//drive away from zone
					driveToPoint({0_in, 0_in});
					break;
				}
			}
			//deploy the tray for teleop
			deployTray();
			break;
		}
		case AutonSelector::Auton::BIG_ZONE_ONE_CUBE:{
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					//push cube into zone	
					driveToPoint({cubeWidth + barrierWidth + 7_in, 0_in});
					//drive away from zone
					driveToPoint({0_in, 0_in});
					break;
				}
				case AutonSelector::Color::RED:{
					//push cube into zone
					driveToPoint({-(cubeWidth + barrierWidth + 7_in), 0_in});
					//drive away from zone
					driveToPoint({0_in, 0_in});
					break;
				}
			}
			//deploy the tray for teleop
			deployTray();
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_5STACK:{
			
			//push preload
			driveToPoint({0_in, 10_in}, 0_deg, 1, 2_in);
			
			//deploy after driving backwards
			rollerOuttake();
			driveToPoint({0_in, 0_in}, 0_deg, 1, 2_in);
			pros::delay(1500);
			rollerIntake();

			//slowly pick up the line of cubes
			model->setMaxVoltage(4000);
			driveToPoint({0_in, 1.5_tile});
			model->setMaxVoltage(12000);

			tilter.setMiddle();
			//drive to zone
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					driveToPoint({0_in, 1_tile - 6_in}, -135_deg, 3, 3_in, 10_deg);

					//stop the rollers at the end of the line
					rollerStop();

					driveToPoint({-4.5_in, 1_tile/2 + 1_in}, -135_deg, 1, 3_in);
					break;
				}
				case AutonSelector::Color::RED:{
					driveToPoint({0_in, 1_tile - 6_in}, 135_deg, 3, 3_in, 10_deg);

					//stop the rollers at the end of the line
					rollerStop();

					driveToPoint({6_in, 1_tile/2 + 1_in}, 135_deg, 1, 3_in);
					break;
				}
			}

			placeStack();
				
			//slow so as to not knock over the stack
			model->setMaxVoltage(4000);
			//back up
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					driveToPoint({6_in, 1_tile + 5_in}, -135_deg, 1, 3_in, 10_deg);
					model->setMaxVoltage(12000);
					tilter.setDown();
					//drive to next location
					driveToPoint({1_tile, 0.5_tile});
					break;
				}
				case AutonSelector::Color::RED:{
					driveToPoint({-6_in, 1_tile + 5_in}, 135_deg, 1, 3_in, 10_deg);
					model->setMaxVoltage(12000);
					tilter.setDown();
					//drive to next location
					driveToPoint({-1_tile, 0.5_tile});
					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_6STACK:{
			driveToPoint({0_in, 13_in});
			//deploy tray
			driveToPoint({0_in, 0_in});
			rollerOuttake();
			pros::delay(1200);

			//slow intake
			rollerOuttake(-1);
			
			//slowly pick up the line 
			model->setMaxVoltage(6000);

			driveToPoint({0_in, 1.6_tile});

			model->setMaxVoltage(8000);

			driveToPoint({0_in, 1.5_tile});
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					//grab 6th cube
					driveToPoint({12_in, 1.5_tile}, 0_deg, 1, 3_in);
					rollerOuttake(-0.75);
					driveToPoint({12_in, 2.05_tile}, 0_deg, 1, 3_in);
					pros::delay(600);
					model->setMaxVoltage(12000);
					//tilter.setMiddle();
					
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.3);

					driveToPoint({-(10_in - 5_in), -1_in + 5_in}, -135_deg, 1.5, 3_in, 10_deg);

					rollerOuttake(-0.2);

					//line up to place
					driveToPoint({-11.5_in, -1_in}, -135_deg, 1, 6_in, 10_deg);
					break;
				}
				case AutonSelector::Color::RED:{
					//grab 6th cube
					driveToPoint({-10_in, 1.5_tile}, 0_deg, 1, 3_in);
					rollerOuttake(-0.55);
					driveToPoint({-10_in, 2.05_tile}, 0_deg, 1, 3_in);
						driveToPoint({-10_in, 1.8_tile}, 0_deg, 1, 3_in);
					pros::delay(600);
					model->setMaxVoltage(12000);
					//tilter.setMiddle();
					
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.3);

					driveToPoint({10_in - 5_in, -1_in + 5_in}, 135_deg, 1.5, 3_in, 10_deg);

					rollerOuttake(-0.2);

					//line up to place
					driveToPoint({9_in, -4.75_in}, 135_deg, 1, 6_in, 10_deg);
					break;
				}
			}	

			placeStack();
				
			//back up
			model->setMaxVoltage(4000);
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					model->setMaxVoltage(6000);
					driveToPoint({-(11.5_in - 15_in), -1_in + 15_in}, -135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);
					tilter.setDown();

					driveToPoint({1_tile, 0.5_tile});
					break;
				}
				case AutonSelector::Color::RED:{
					model->setMaxVoltage(6000);
					driveToPoint({9_in - 15_in, -4.75_in + 15_in}, 135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);

					tilter.setDown();

					driveToPoint({-1_tile, 0.5_tile});
					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_7STACK:{
			driveToPoint({0_in, 14_in});
			//deploy tray
			driveToPoint({0_in, 1_in});
			rollerOuttake();
			pros::delay(2000);

			//slow intake
			rollerIntake();
			
			//slowly pick up the line 
			model->setMaxVoltage(5500);

			driveToPoint({0_in, 1.5_tile});

			model->setMaxVoltage(5500);

			switch(alliance){
				case AutonSelector::Color::BLUE:{
					//grab 6th cube
					driveToPoint({8_in, 1.5_tile}, 0_deg, 1, 3_in);
					driveToPoint({8_in, 2.05_tile}, 0_deg, 1, 3_in);

					driveToPoint({13_in, 1.8_tile}, -45_deg, 1, 3_in);
					driveToPoint({13_in, 2.1_tile}, -45_deg, 1, 3_in);

					pros::delay(300);
					model->setMaxVoltage(12000);
					tilter.setMiddle();
					
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.7);

					driveToPoint({-10_in + 5_in, -1_in + 5_in}, -135_deg, 1.5, 3_in, 10_deg);

					rollerStop();

					//line up to place
					driveToPoint({-10_in, -1_in}, -135_deg, 1, 6_in, 10_deg);
					break;
				}
				case AutonSelector::Color::RED:{
					//grab 6th cube
					driveToPoint({-8_in, 1.5_tile}, 0_deg, 1, 3_in);
					driveToPoint({-8_in, 2.05_tile}, 0_deg, 1, 3_in);

					driveToPoint({-13_in, 1.8_tile}, 45_deg, 1, 3_in);
					driveToPoint({-13_in, 2.1_tile}, 45_deg, 1, 3_in);

					pros::delay(300);
					model->setMaxVoltage(12000);
					tilter.setMiddle();
					
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.7);

					driveToPoint({10_in - 5_in, -1_in + 5_in}, 135_deg, 1.5, 3_in, 10_deg);

					rollerStop();

					//line up to place
					driveToPoint({10_in, -1_in}, 135_deg, 1, 6_in, 10_deg);
					break;
				}
			}	

			placeStack();
				
			//back up
			model->setMaxVoltage(4000);
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					model->setMaxVoltage(6000);
					driveToPoint({-10_in + 15_in, -1_in + 15_in}, -135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);

					tilter.setDown();

					driveToPoint({1_tile, 0.5_tile});
					break;
				}
				case AutonSelector::Color::RED:{
					model->setMaxVoltage(6000);
					driveToPoint({10_in - 15_in, -1_in + 15_in}, 135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);

					tilter.setDown();

					driveToPoint({-1_tile, 0.5_tile});
					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_8STACK:{
			driveToPoint({0_in, 14_in});
			//deploy tray
			driveToPoint({0_in, 1_in});
			rollerOuttake();
			pros::delay(2000);

			//slow intake
			rollerIntake();
			
			//slowly pick up the line 
			model->setMaxVoltage(5500);

			driveToPoint({0_in, 1.5_tile});

			model->setMaxVoltage(12000);

			switch(alliance){
				case AutonSelector::Color::BLUE:{
					driveToPoint({-1_tile, 0_in});
					model->setMaxVoltage(5500);
					driveToPoint({-1_tile, 1.5_tile});

					tilter.setMiddle();
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.7);

					driveToPoint({-10_in + 5_in - 1_tile, -1_in + 5_in}, -135_deg, 1.5, 3_in, 10_deg);

					rollerStop();

					//line up to place
					driveToPoint({-10_in - 1_tile, -1_in}, -135_deg, 1, 6_in, 10_deg);
					break;
				}
				case AutonSelector::Color::RED:{
					driveToPoint({1_tile, 0_in});
					model->setMaxVoltage(5500);
					driveToPoint({1_tile, 1.5_tile});

					tilter.setMiddle();
					//slowly intake so as to keep the cubes from dropping
					rollerOuttake(-0.7);

					driveToPoint({10_in - 5_in + 1_tile, -1_in + 5_in}, 135_deg, 1.5, 3_in, 10_deg);

					rollerStop();

					//line up to place
					driveToPoint({10_in + 1_tile, -1_in}, 135_deg, 1, 6_in, 10_deg);
					break;
				}
			}	

			placeStack();
				
			//back up
			model->setMaxVoltage(4000);
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					model->setMaxVoltage(6000);
					driveToPoint({-10_in + 15_in - 1_tile, -1_in + 15_in}, -135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);

					tilter.setDown();

					break;
				}
				case AutonSelector::Color::RED:{
					model->setMaxVoltage(6000);
					driveToPoint({10_in - 15_in + 1_tile, -1_in + 15_in}, 135_deg, 1.5, 3_in, 10_deg);
					model->setMaxVoltage(12000);

					tilter.setDown();

					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::BIG_ZONE_3STACK:{
			driveToPoint({0_in, 14_in});
			//deploy tray
			driveToPoint({0_in, 1_in});
			rollerOuttake();
			pros::delay(1700);
			//intake
			rollerIntake();
			//drive forward, grab cube 
			driveToPoint({0_in, 1_tile}); 
			//turn around and grab cube by zone
			//slow intake
			//may need to tune Y value
			rollerOuttake(-0.7);

			model->setMaxVoltage(10000);
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-6_in, 36_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({6_in, 36_in}, 135_deg); 
					break;
				}
			}
			//line up
			//definitely needs tuned
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-31_in, 8_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({30_in, 10_in}, 135_deg); 
					break;
				}
			}
			
			//place
			placeStack();
			model->setMaxVoltage(4000);
			//back up
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-31_in + 15_in, 8_in + 15_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg); 
					break;
				}
			}
			break;
			tilter.setDown();
		}
		case AutonSelector::Auton::BIG_ZONE_LARGESTACK:{
			driveToPoint({0_in, 14_in});
			//deploy tray
			driveToPoint({0_in, 1_in});
			rollerOuttake();
			pros::delay(1700);
			//intake
			rollerIntake();
			//drive forward, grab cube 
			model->setMaxVoltage(10000);
			driveToPoint({0_in, 1.5_tile}); 
			driveToPoint({0_in, 1.4_tile}); 
			model->setMaxVoltage(6000);
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-14_in, 1.5_tile}, 0_deg, 1, 4_in);
					rollerOuttake(-0.75);
					driveToPoint({-14_in, 2_tile}, 0_deg, 1, 4_in);
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({12_in, 1.5_tile}, 0_deg, 1, 4_in);
					rollerOuttake(-0.75);
					driveToPoint({12_in, 2_tile}, 0_deg, 1, 4_in);
					break;
				}
			}
			//turn around and grab cube by zone
			//slow intake
			//may need to tune Y value
			rollerOuttake(-0.3);

			model->setMaxVoltage(8000);
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-6_in + 10_in, 36_in + 10_in}, -135_deg); 

					rollerIntake();
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({6_in, 36_in}, 135_deg); 
					break;
				}
			}
			//line up
			//definitely needs tuned
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-28.25_in, 7.25_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({30_in, 10_in}, 135_deg); 
					break;
				}
			}
			rollerStop();
			//place
			placeStack();
			model->setMaxVoltage(4000);
			//back up
			switch(alliance){
				case AutonSelector::Color::RED:{
					driveToPoint({-28_in + 15_in, 7.5_in + 15_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg); 
					break;
				}
			}
			break;
			tilter.setDown();
		}
		case AutonSelector::Auton::BIG_ZONE_PUSH:{
			//push cube into zone
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					driveToPoint({cubeWidth + barrierWidth + 11_in, 0_in});
					break;
				}
				case AutonSelector::Color::RED:{
					driveToPoint({-(cubeWidth + barrierWidth + 11_in), 0_in});
					break;
				}
			}
			//line up to push 2nd cube
			driveToPoint({0_in, 0_in});
			driveToPoint({0_in, 1.4_tile});

			model->setMaxVoltage(12000);
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					driveToPoint(
						{cubeWidth + barrierWidth + 9_in,
						bigZoneLength + barrierWidth - 9_in }, 
						55_deg);
					break;
				}
				case AutonSelector::Color::RED:{
					driveToPoint({-(cubeWidth + barrierWidth + 9_in), 
							bigZoneLength + barrierWidth - 9_in},
							-55_deg);
					break;
				}
			}
			//reset
			model->setMaxVoltage(12000);
			driveToPoint({0_in, 0_in});
			
			deployTray();
			break;
		}
		default:
			break;
	}
}


Stacker stacker{*Stacker::getInstance()};

//used for toggle presses
LatchedBoolean left{};
LatchedBoolean right{};
LatchedBoolean offsetPressForward{};
LatchedBoolean offsetPressBackward{};
LatchedBoolean toggleSpeed{}; 
/*
 * Methods
 * Drive *
 * Turn * 
 * Tilter up *
 * Tilter down *
 * Tilter middle? *
 * Rollers in *
 * Rollers out *
 * Rollers slower?
 * Arms manual up *
 * Arms manual down *
 * Arms down *
 * Arms 1 *
 * Arms 2 *
 * Arms 3 *
 *
 * Controls
 * L Bumper Arms manual up
 * L Trigger All tilter
 * R Bumper Rollers Outtake
 * R Trigger Rollers Intake
 * Joystick 1 Turn, flick up to deploy
 * Joystick 2 Drive
 * X Arms 3
 * Y Arms 2
 * A
 * B
 * up Arms manual down
 * down Arms down
 * left Arms 1
 * right Arms at setting that works for 2 and 3
 *
 *
 * set arms brake mode to hold
 */
void opcontrol() {
	model->setMaxVoltage(12000);
  	while (true) {	
		//maps the drive inputs into curves, making them easier to finely control
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

		//drives the bot
		model->xArcade(xPower, yPower, turn);

		//control the rollers
		if(controller.getDigital(ControllerDigital::left)){
			stacker.slowOuttake();
			stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
			stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
		}else if(controller.getDigital(ControllerDigital::R1)){
			stacker.outtake();
			stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
			stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
		}else if(controller.getDigital(ControllerDigital::R2)){
			stacker.intake();
			stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
			stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
		}else{
			stacker.stop();
			stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::hold);
			stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::hold);
		}

		stacker.in();
		stacker.out();
		
		tilter.in();
		tilter.out();

		controller.setText(0, 0, std::to_string(tilter.getTrayAngle()));

		
		//control the tilter
		if(left.update(controller.getDigital(ControllerDigital::L2))){
			tilter.shiftDown();
		}
		if(right.update(controller.getDigital(ControllerDigital::L1))){
			tilter.shiftUp();
			stackerMotor1->setBrakeMode(AbstractMotor::brakeMode::coast);
			stackerMotor2->setBrakeMode(AbstractMotor::brakeMode::coast);
		}else if(controller.getDigital(ControllerDigital::down)){
			//adjusts once the tilter is up
			tilter.adjustThrottle(-127 * 0.5);
		}else{
			tilter.adjustThrottle(0);
		}

		//used to offset the tilter in case of skipping
		if(offsetPressForward.update(controller.getDigital(ControllerDigital::B))){
			tilter.offsetForward();
		}
		if(offsetPressBackward.update(controller.getDigital(ControllerDigital::X))){
			tilter.offsetBackward();
		}

		stackerMotor1->moveVoltage(stacker.getOutput());
		stackerMotor2->moveVoltage(stacker.getOutput());

		//slow the speed of the drivetrain for moving stacks
		if(toggleSpeed.update(controller.getDigital(ControllerDigital::Y))){
			if(model->getMaxVoltage() == 12000){
				model->setMaxVoltage(4000);	
			}else{
				model->setMaxVoltage(12000);
			}
		}
    		pros::delay(10);
	}
}
