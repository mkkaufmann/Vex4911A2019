#include "subsystems/intake.hpp"

void Intake::setSpeed(double percent){
	rollers.moveVoltage(percent * 12000);
}

void Intake::outtake(){
	setSpeed(1);
}

void Intake::intake(){
	setSpeed(-1);
}

void Intake::stop(){
	setSpeed(0);
}

Intake& Intake::getInstance(){
	return instance;
}

Intake::Intake():rollers(okapi::MotorGroup({6,-5})){};


