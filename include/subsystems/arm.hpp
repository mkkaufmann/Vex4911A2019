#pragma once
#include "constants.hpp"
#include "main.h"

extern okapi::Motor armMotor;
extern std::shared_ptr<okapi::AsyncPositionController<double, double>> armPID;

void initializeArm();

void setArmAngle(double potUnits);

void setArmDown();
void setArmAlliance();
void setArmHighMiddle();
void setArmLowMiddle();

void setArmManualSpeed(double speed);
