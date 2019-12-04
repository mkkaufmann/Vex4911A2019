#include "subsystems/robot.hpp" 

Robot& Robot::getInstance(){
	return instance;
}

void Robot::initialize(){

}

std::shared_ptr<ThreeEncoderXDriveModel> Robot::getModel(){
	return model;
}
	
std::shared_ptr<CustomOdometry> Robot::getOdom(){
	return odom;
}

std::shared_ptr<OdomXController> Robot::getOdomController(){
	return odomController;
}

Tilter& Robot::getTilter(){
	return tilter;
}

Intake& Robot::getIntake(){
	return intake;
}

void Robot::placeStack(){
	//implement
}

Robot::Robot() : tilter(Tilter::getInstance()), intake(Intake::getInstance()){
	//This is the model of the robot chassis
	//It keeps track of the motors, encoder wheels, max Velocity (in RPM), and max Voltage
	model = std::make_shared<ThreeEncoderXDriveModel>(
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
	);	

	//keeps track of position and orientation based on bot measurements
	odom = std::make_shared<CustomOdometry>(
		model,
		ChassisScales({
			2.792_in,//Encoder wheel diameter
			14.0006164_in,//Drive base width, assumes encoder wheels are the same distance from the center
			6.50_in//Middle wheel distance, which doesn't matter to the math
		}, 360)//Encoder ticks per rotation
	);

	odomController = std::make_shared<OdomXController>(
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
		//Strafe PID - To mm
		//Deprecated
		std::make_unique<IterativePosPIDController>(
			0.5, 0.000, 0.1, 0,
			TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)
		),
		2_in//this is how close the bot has to be to the target in order to settle
	);
}

Robot Robot::instance = Robot::getInstance();
