#pragma once
#include "main.h"
#include "constants.hpp"

extern okapi::Motor armMotor; 
extern const double encoderTicksPerDegree;

void initializeArm();

void setArmAngle(double potUnits);

void setArmDown();
void setArmAlliance();
void setArmHighMiddle();
void setArmLowMiddle();

void setArmManualSpeed(double speed);
