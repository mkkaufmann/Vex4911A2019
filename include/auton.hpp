#pragma once
#include "lib7842/api.hpp"
#include "main.h"

using namespace lib7842;
using namespace lib7842::units;
using namespace okapi;

extern std::shared_ptr<Motor> topLeft;
extern std::shared_ptr<Motor> topRight;
extern std::shared_ptr<Motor> bottomRight;
extern std::shared_ptr<Motor> bottomLeft;

extern std::shared_ptr<Motor> stackerMotor1;
extern std::shared_ptr<Motor> stackerMotor2;

extern std::shared_ptr<ThreeEncoderXDriveModel> model;
extern ChassisScales odomChassisScales;
extern ChassisScales driveChassisScales;
extern std::shared_ptr<CustomOdometry> odom;
extern std::shared_ptr<OdomXController> odomController;
extern PathFollower follower;
extern PursuitLimits fullSpeedLimits;
extern PursuitLimits halfSpeedLimits;

void rollerOuttake();
void rollerOuttake(double t);
void rollerIntake();
void rollerStop();

void deploy();
void placeStack(double ms);
void driveToPoint(
    Vector target, QAngle targetAngle = 0_deg, double turnPriority = 1,
    Settler &&settler = Settler().distanceErr(4_in).angleErr(5_deg));

void driveToPoint(Vector target, Vector pointToFace, double turnPriority = 1,
                  QLength settleDistance = 4_in, QAngle settleAngle = 5_deg);

void oneCube();
void fiveCubeRed();
void fiveCubeBlue();
void fiveCubeBluePP();
void sixCubeRed();
void sixCubeBlue();
void sevenCubeRed();
void sevenCubeBlue();
void nineCubeRed();
void nineCubeBlue();
void threeCubeBigRed();
void threeCubeBigBlue();
void fourCubeBigRed();
void fourCubeBigBlue();
