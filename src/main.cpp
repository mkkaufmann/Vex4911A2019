#include "main.h"
#include "subsystems/robot.hpp"
#include "util/util.hpp"
#include "ui/autonselector.hpp"
#include "util/latchedboolean.hpp"

using namespace okapi;

Controller controller{ControllerId::master};

Screen scr{lv_scr_act(), LV_COLOR_RED};

auto robot {Robot::instance};
auto odom{robot->getOdom()};
Tilter tilter = robot->getTilter();
Intake intake = robot->getIntake();

//Constant distances used for autonomous plotting
const QLength botWidth {17.5_in};
const QLength botLength {17.5_in};

const QLength cubeWidth {5.5_in};
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
	scr.makePage<OdomDebug>().attachOdom(odom);
	scr.startTask("Screen");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	odom->reset();
	robot->setSpeed(1);
	
	//Get the chosen alliance and auton
	alliance = AutonSelector::getColor();
	currentAuton = AutonSelector::getAuton(); 

	tilter.autoDown();
	switch(currentAuton){
		case AutonSelector::Auton::TEST:{
			tilter.autoUp();
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_ONE_CUBE:{
			switch(alliance){
				case AutonSelector::Color::RED:{
					//push cube into zone
					robot->driveToPoint({cubeWidth + barrierWidth + 7_in, 0_in});
					//drive away from zone
					robot->driveToPoint({0_in, 0_in});
					break;
				}
				case AutonSelector::Color::BLUE:{
					//push cube into zone
					robot->driveToPoint({-(cubeWidth + barrierWidth + 7_in), 0_in});
					//drive away from zone
					robot->driveToPoint({0_in, 0_in});
					break;
				}
			}
			//deploy the tray for teleop
			robot->deployTray();
			break;
		}
		case AutonSelector::Auton::BIG_ZONE_ONE_CUBE:{
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					//push cube into zone	
					robot->driveToPoint({cubeWidth + barrierWidth + 7_in, 0_in});
					//drive away from zone
					robot->driveToPoint({0_in, 0_in});
					break;
				}
				case AutonSelector::Color::RED:{
					//push cube into zone
					robot->driveToPoint({-(cubeWidth + barrierWidth + 7_in), 0_in});
					//drive away from zone
					robot->driveToPoint({0_in, 0_in});
					break;
				}
			}
			//deploy the tray for teleop
			robot->deployTray();
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_5STACK:{
			
			//push preload
			robot->driveToPoint({0_in, 10_in}, 0_deg, 1, 2_in);
			
			//deploy after driving backwards
			intake.outtake();
			robot->driveToPoint({0_in, 0_in}, 0_deg, 1, 2_in);
			pros::delay(1500);
			intake.intake();
			//slowly pick up the line of cubes
			robot->setSpeed(0.33);
			robot->driveToPoint({0_in, 1.5_tl});
			robot->setSpeed(1);

			//drive to zone
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({0_in, 1_tl - 6_in}, -135_deg, 3, 3_in, 10_deg);

					//stop the rollers at the end of the line
					intake.stop();

					robot->driveToPoint({-4.5_in, 1_tl/2 + 1_in}, -135_deg, 1, 3_in);
					break;
				}
				case AutonSelector::Color::RED:{
					robot->driveToPoint({0_in, 1_tl - 6_in}, 135_deg, 3, 3_in, 10_deg);

					//stop the rollers at the end of the line
					intake.stop();

					robot->driveToPoint({6_in, 1_tl/2 + 1_in}, 135_deg, 1, 3_in);
					break;
				}
			}

			robot->placeStack();
				
			//slow so as to not knock over the stack
			robot->setSpeed(0.33);
			//back up
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({6_in, 1_tl + 5_in}, -135_deg, 1, 3_in, 10_deg);
					robot->setSpeed(1);
					tilter.autoDown();
					//drive to next location
					robot->driveToPoint({1_tl, 0.5_tl});
					break;
				}
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-6_in, 1_tl + 5_in}, 135_deg, 1, 3_in, 10_deg);
					robot->setSpeed(1);
					tilter.autoDown();
					//drive to next location
					robot->driveToPoint({-1_tl, 0.5_tl});
					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_6STACK:{
			robot->driveToPoint({0_in, 14_in});
			//deploy tray
			robot->driveToPoint({0_in, 1_in});
			intake.outtake();
			pros::delay(2000);

			//slow intake
			intake.intake();
			
			//slowly pick up the line 
			robot->setSpeed(0.46);

			robot->driveToPoint({0_in, 1.5_tl});

			switch(alliance){
				case AutonSelector::Color::BLUE:{
					//grab 6th cube
					robot->driveToPoint({8_in, 1.5_tl}, 0_deg, 1, 3_in);
					robot->driveToPoint({8_in, 2.05_tl}, 0_deg, 1, 3_in);

					pros::delay(300);
					robot->setSpeed(1);
					
					//slowly intake so as to keep the cubes from dropping
					intake.setSpeed(-0.7);

					robot->driveToPoint({-10_in + 5_in, -1_in + 5_in}, -135_deg, 1.5, 3_in, 10_deg);

					intake.stop();

					//line up to place
					robot->driveToPoint({-10_in, -1_in}, -135_deg, 1, 6_in, 10_deg);
					break;
				}
				case AutonSelector::Color::RED:{
					//grab 6th cube
					robot->driveToPoint({-8_in, 1.5_tl}, 0_deg, 1, 3_in);
					robot->driveToPoint({-8_in, 2.05_tl}, 0_deg, 1, 3_in);

					pros::delay(300);
					robot->setSpeed(1);
					
					//slowly intake so as to keep the cubes from dropping
					intake.setSpeed(-0.7);

					robot->driveToPoint({10_in - 5_in, -1_in + 5_in}, 135_deg, 1.5, 3_in, 10_deg);

					intake.stop();

					//line up to place
					robot->driveToPoint({10_in, -1_in}, 135_deg, 1, 6_in, 10_deg);
					break;
				}
			}	

			robot->placeStack();
				
			//back up
			robot->setSpeed(0.33);
			switch(alliance){
				case AutonSelector::Color::BLUE:{
					robot->setSpeed(0.5);
					robot->driveToPoint({-10_in + 15_in, -1_in + 15_in}, -135_deg, 1.5, 3_in, 10_deg);
					robot->setSpeed(1);

					tilter.autoDown();

					robot->driveToPoint({1_tl, 0.5_tl});
					break;
				}
				case AutonSelector::Color::RED:{
					robot->setSpeed(0.5);
					robot->driveToPoint({10_in - 15_in, -1_in + 15_in}, 135_deg, 1.5, 3_in, 10_deg);
					robot->setSpeed(1);

					tilter.autoDown();

					robot->driveToPoint({-1_tl, 0.5_tl});
					break;
				}
			}
			break;
		}
		case AutonSelector::Auton::SMALL_ZONE_7STACK:{
			break;
		}
		case AutonSelector::Auton::BIG_ZONE_3STACK:{
			robot->driveToPoint({0_in, 14_in});
			//deploy tray
			robot->driveToPoint({0_in, 1_in});
			intake.outtake();
			pros::delay(1700);
			//intake
			intake.intake();
			//drive forward, grab cube 
			robot->driveToPoint({0_in, 1_tl}); 
			//turn around and grab cube by zone
			//slow intake
			//may need to tune Y value
			robot->setSpeed(-0.7);

			robot->setSpeed(0.83);
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToFacePoint({-12_in, 2_tl}, {-1_tl, 10_in}); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToFacePoint({12_in, 2_tl}, {1_tl, 10_in}); 
					break;
				}
			}
			//line up
			//definitely needs tuned
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-27_in, 20.5_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({27_in, 20.5_in}, 135_deg); 
					break;
				}
			}
			
			//place
			robot->placeStack();
			robot->setSpeed(0.33);
			//back up
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-27_in + 15_in, 20.5_in + 15_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg); 
					break;
				}
			}
			break;
			tilter.autoDown();
		}
		case AutonSelector::Auton::BIG_ZONE_LARGESTACK:{
			robot->driveToPoint({0_in, 14_in});
			//deploy tray
			robot->driveToPoint({0_in, 1_in});
			intake.outtake();
			pros::delay(1700);
			//intake
			intake.setSpeed(-0.7);
			//drive forward, grab cube and stack
			robot->driveToPoint({0_in, 1_tl}); 
			robot->setSpeed(0.42);
			robot->driveToPoint({0_in, 1.3_tl}); 
			//grab cube by tower
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-10_in, 1.5_tl}); 
					robot->driveToPoint({-10_in, 2.05_tl}); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({10_in, 1.5_tl}); 
					robot->driveToPoint({10_in, 2.05_tl}); 
					break;
				}
			}
			robot->setSpeed(1);
			//turn around and grab cube by zone
			//slow intake
			//may need to tune Y value
			intake.intake();
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-5_in, 1.8_tl}, -135_deg); 
					robot->driveToPoint({-12_in, 36.2_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({5_in, 1.8_tl}, 135_deg); 
					robot->driveToPoint({12_in, 36.2_in}, 135_deg); 
					break;
				}
			}
			intake.intake();
			pros::delay(500);
			//line up
			//definitely needs tuned
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-27_in , 20.5_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({27_in , 20.5_in}, 135_deg); 
					break;
				}
			}
			//place
			robot->placeStack();
			//back up
			robot->setSpeed(0.33);
			switch(alliance){
				case AutonSelector::Color::RED:{
					robot->driveToPoint({-27_in + 15_in, 20.5_in + 15_in}, -135_deg); 
					break;
				}
				case AutonSelector::Color::BLUE:{
					robot->driveToPoint({27_in - 15_in, 20.5_in + 15_in}, 135_deg); 
					break;
				}
			}
			tilter.autoDown();
			break;
		}
		default:
			break;
	}
}



//used for toggle presses
LatchedBoolean left{};
LatchedBoolean right{};
LatchedBoolean offsetPressForward{};
LatchedBoolean offsetPressBackward{};
LatchedBoolean toggleSpeed{}; 

void opcontrol() {
	robot->setSpeed(1);
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
		robot->drive(xPower, yPower, turn);

		//control the rollers
		if(controller.getDigital(ControllerDigital::left)){
			intake.setSpeed(0.5);
		}else if(controller.getDigital(ControllerDigital::R1)){
			intake.outtake();
		}else if(controller.getDigital(ControllerDigital::R2)){
			intake.intake();
		}else{
			intake.stop();
		}

		tilter.loop();

		//control the tilter
		if(left.update(controller.getDigital(ControllerDigital::L2))){
			tilter.down();
		}
		if(right.update(controller.getDigital(ControllerDigital::L1))){
			tilter.up();
		}

		//used to offset the tilter in case of skipping
		if(offsetPressForward.update(controller.getDigital(ControllerDigital::B))){
			tilter.offsetForward();
		}
		if(offsetPressBackward.update(controller.getDigital(ControllerDigital::X))){
			tilter.offsetBackward();
		}


		//slow the speed of the drivetrain for moving stacks
		if(toggleSpeed.update(controller.getDigital(ControllerDigital::Y))){
			robot->toggleSlow();
		}
    		pros::delay(10);
	}
}
