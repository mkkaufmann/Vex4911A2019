#include "subsystems/arm.hpp"

okapi::Motor armMotor = okapi::Motor(Constants::ARM_MOTOR_PORT);

void initializeArm(){
		
};

void setAngle(double potUnits);

void setDown();
void setAlliance();
void setHighMiddle();
void setLowMiddle();
