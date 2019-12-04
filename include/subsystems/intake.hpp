#pragma once
#include "main.h"
class Intake{
public:
	void setSpeed(double percent);
	void outtake();
	void intake();
	void stop();
	static Intake& getInstance();
private:
	static Intake instance;
	okapi::MotorGroup rollers;	
	Intake();
};

