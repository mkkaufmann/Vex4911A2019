#include "main.h"
#include "constants.hpp"
#include "subsystems/oi.hpp"
#include "subsystems/positiontracker.hpp"
#include "subsystems/drive.hpp"
#include "subsystems/stacker.hpp"
#include "motionprofiles/pathfollower.hpp"
#include "util/point.hpp"
#include "util/latchedboolean.hpp"
#include "util/util.hpp"
#include "util/actions.hpp"
#include "pros/apix.h"
#include "ui/ui.hpp"
#include "ui/uihelper.hpp"

//starting orientations
double facingForward = Util::toRadians(0);
double facingLeft = Util::toRadians(90);//not sure
double facingRight = Util::toRadians(180);
double facingBackward = Util::toRadians(270);

//initialize static values

//ports
int Constants::LEFT_FRONT_MOTOR_PORT = 1;
int Constants::LEFT_REAR_MOTOR_PORT = 2;
int Constants::RIGHT_FRONT_MOTOR_PORT = 3;
int Constants::RIGHT_REAR_MOTOR_PORT = 4;
int Constants::STACKER_TREAD_2_MOTOR_PORT = 5;
int Constants::STACKER_TREAD_1_MOTOR_PORT = 6;

int Constants::LEFT_TRACKING_ENCODER_TOP = 1;
int Constants::LEFT_TRACKING_ENCODER_BOTTOM = 2;
int Constants::RIGHT_TRACKING_ENCODER_TOP = 3;
int Constants::RIGHT_TRACKING_ENCODER_BOTTOM = 4;
int Constants::REAR_TRACKING_ENCODER_TOP = 5;
int Constants::REAR_TRACKING_ENCODER_BOTTOM = 6;

//measured constants for odometry
double Constants::LEFT_WHEEL_TO_TRACKING_CENTER = 6.5;
double Constants::RIGHT_WHEEL_TO_TRACKING_CENTER = 7.25;
double Constants::REAR_WHEEL_TO_TRACKING_CENTER = 7.75;
int Constants::ENCODER_TICKS_PER_INCH = 360/(2.75 * M_PI);

//initialize singletons
OI* OI::instance = OI::getInstance();
Drive* Drive::instance = Drive::getInstance();
Stacker* Stacker::instance = Stacker::getInstance();
PositionTracker* PositionTracker::instance = PositionTracker::getInstance();

//initialize odometry values
double PositionTracker::pLeftEnc = 0;
double PositionTracker::pRightEnc = 0;
double PositionTracker::pBackEnc = 0;
double PositionTracker::pTheta = facingForward;//change this to starting orientation
double PositionTracker::leftEncAtLastReset = 0;
double PositionTracker::rightEncAtLastReset = 0;
double PositionTracker::thetaAtLastReset = PositionTracker::pTheta;
Point PositionTracker::globalPosition = Point(0,0);

//set initial drive setting
bool Drive::fieldCentric = true;
//used for changing the above setting during a match
LatchedBoolean Drive::fieldCentricToggle = LatchedBoolean();

//number of loops for PID to count as settled
double PathFollower::iterationsForSettle = 5;//100ms



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
 UIHelper::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
