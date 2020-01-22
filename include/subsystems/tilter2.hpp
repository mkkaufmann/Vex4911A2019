#pragma once
#include "main.h"
#include "constants.hpp"

extern okapi::Motor tilterMotor; 
extern const double MIDDLE_HEIGHT;
extern const double UP_HEIGHT;
extern std::shared_ptr<okapi::AsyncPositionController<double, double>> tilterPID;

void initializeTilter();

void setTilterUp();
void setTilterDown();
void setTilterMiddle();
void setTilterTowers();
void setTilterManeuver();
void setTilterDisabled();

