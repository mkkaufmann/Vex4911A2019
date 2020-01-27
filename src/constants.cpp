#include "constants.hpp"
#include <cmath>

// initialize static values
// ports
int Constants::LEFT_FRONT_MOTOR_PORT = 4;
int Constants::LEFT_REAR_MOTOR_PORT = 6;
int Constants::RIGHT_FRONT_MOTOR_PORT = -5;
int Constants::RIGHT_REAR_MOTOR_PORT = -7;
int Constants::STACKER_TREAD_2_MOTOR_PORT = 10;
int Constants::STACKER_TREAD_1_MOTOR_PORT = -9;
int Constants::TILTER_MOTOR_PORT = -2;
int Constants::ARM_MOTOR_PORT = -3;

int Constants::LEFT_TRACKING_ENCODER_TOP = 1;
int Constants::LEFT_TRACKING_ENCODER_BOTTOM = 2;
int Constants::RIGHT_TRACKING_ENCODER_TOP = 3;
int Constants::RIGHT_TRACKING_ENCODER_BOTTOM = 4;
int Constants::REAR_TRACKING_ENCODER_TOP = 5;
int Constants::REAR_TRACKING_ENCODER_BOTTOM = 6;

// measured constants for odometry
double Constants::LEFT_WHEEL_TO_TRACKING_CENTER = 6.5;
double Constants::RIGHT_WHEEL_TO_TRACKING_CENTER = 7.25;
double Constants::REAR_WHEEL_TO_TRACKING_CENTER = 7;
int Constants::ENCODER_TICKS_PER_INCH = 360 / (2.75 * M_PI);
