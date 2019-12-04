#pragma once
#include "lib7842/api.hpp"
#include "main.h"
#include "tilter.hpp"
#include "intake.hpp"
using namespace okapi;
using namespace lib7842;
class Robot{
public:
	static Robot instance;
	static void initialize();
	std::shared_ptr<ThreeEncoderXDriveModel> getModel();
	std::shared_ptr<CustomOdometry> getOdom();
	std::shared_ptr<OdomXController> getOdomController();
	Tilter& getTilter();
	Intake& getIntake();

	void drive(double x, double y, double turn);	
	void setSpeed(double percent);
	void placeStack();
	void deployTray();

	void toggleSlow();
	
	void driveToPoint(
			Vector target, 
			QAngle targetAngle=0_deg, 
			double turnPriority=1, 
			QLength settleDistance=4_in, 
			QAngle settleAngle=5_deg);

	void driveToFacePoint(
			Vector target, 
			Vector pointToFace, 
			double turnPriority=1, 
			QLength settleDistance=4_in, 
			QAngle settleAngle=5_deg);
private:
	Robot();
	std::shared_ptr<ThreeEncoderXDriveModel> model;
	std::shared_ptr<CustomOdometry> odom;
	std::shared_ptr<OdomXController> odomController;
	Tilter& tilter;
	Intake& intake;
};
