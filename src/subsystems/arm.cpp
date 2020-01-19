#include "subsystems/arm.hpp"

okapi::Motor armMotor = okapi::Motor(Constants::ARM_MOTOR_PORT);
std::shared_ptr<okapi::AsyncPositionController<double, double>> armPID = 
okapi::AsyncPosControllerBuilder()
	.withMotor(armMotor)
	.withGains(okapi::IterativePosPIDController::Gains{0.0,0.0,0.0})
	.withGearset(okapi::AbstractMotor::GearsetRatioPair(okapi::AbstractMotor::gearset::green, 7))
	.build();

void initializeArm(){
	armPID->tarePosition();
};

void setArmPosition(double position){
	armPID->setTarget(position);
}

void setArmDown(){
	armPID->setTarget(0);
}

void setArmAlliance(){
	armPID->setTarget(0);
}

void setArmHighMiddle(){
	armPID->setTarget(0);
}

void setArmLowMiddle(){
	armPID->setTarget(0);
}

void setArmManualSpeed(double speed){
	armMotor.moveVoltage(12000 * speed);
}
