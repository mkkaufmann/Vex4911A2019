#include "subsystems/arm.hpp"

okapi::Motor armMotor = okapi::Motor(Constants::ARM_MOTOR_PORT);
const double ENCODER_TICKS_PER_DEGREE = (900.0 * 7)/360;

void initializeArm(){
	armMotor.setPosPID(0.0, 0.0, 0.0, 0.0);	
	armMotor.tarePosition();
};

void setArmAngle(double angle){
	armMotor.moveAbsolute(angle * ENCODER_TICKS_PER_DEGREE, 12000);
}

void setArmDown(){
	setArmAngle(0);
}

void setArmAlliance(){
	setArmAngle(30);
}

void setArmHighMiddle(){
	setArmAngle(50);
}

void setArmLowMiddle(){
	setArmAngle(60);
}

void setArmManualSpeed(double speed){
	armMotor.moveVoltage(12000 * speed);
}
