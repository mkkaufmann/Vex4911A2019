#pragma once
#include "main.h"
#include "constants.hpp"

extern okapi::Motor armMotor; 

void initializeArm();

void setAngle(double potUnits);

void setDown();
void setAlliance();
void setHighMiddle();
void setLowMiddle();
