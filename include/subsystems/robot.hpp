#pragma once
#include "lib7842/api.hpp"
#include "main.h"
#include "tilter.hpp"
#include "intake.hpp"
using namespace okapi;
using namespace lib7842;
class Robot{
public:
	static Robot& getInstance();
	static void initialize();
	std::shared_ptr<ThreeEncoderXDriveModel> getModel();
	std::shared_ptr<CustomOdometry> getOdom();
	std::shared_ptr<OdomXController> getOdomController();
	Tilter& getTilter();
	Intake& getIntake();

	void placeStack();
private:
	static Robot instance;
	Robot();
	std::shared_ptr<ThreeEncoderXDriveModel> model;
	std::shared_ptr<CustomOdometry> odom;
	std::shared_ptr<OdomXController> odomController;
	Tilter& tilter;
	Intake& intake;
};
