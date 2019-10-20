#ifndef DRIVE_HPP
#define DRIVE_HPP
#include "subsystem.hpp"
#include "main.h"
#include "constants.hpp"
#include "util/polarpoint.hpp"
#include "util/point.hpp"
#include "util/pointutil.hpp"
#include "positiontracker.hpp"
#include "util/latchedboolean.hpp"
//drivetrain subsystem
class Drive : public Subsystem{
public:

	//return singleton
  static Drive* getInstance();

	//stop driving
  void stop();

	void drivePath(int x, int y, int r);

	void driveManually(int x, int y, int r);

  void drivePseudoManual(int x, int y, int r);

  std::function<void()> driveManuallyAction(int x, int y, int r, bool fieldCentric);
  // void driveWhileSeekingRotationTarget(int x, int y, double theta){
  //
  // }

  std::function<void()> driveTowardsPointAction(double x, double y);
  //radians measured counterclockwise??
  std::function<void()> driveTowardsPointAndOrientationAction(double x, double y, double radians);


  //used for changing between field and robot centric
	static void updateFieldCentric(bool pressed);

  void in();

  std::function<void()> inAction();
	//run motors
  void out();

  std::function<void()> outAction();

  enum DriveState {
    NEUTRAL,
    MANUAL,
		PATH_FOLLOWING
  };
private:
	//initialize motors
	pros::Motor leftFrontMotor = pros::Motor(Constants::LEFT_FRONT_MOTOR_PORT);
	pros::Motor leftRearMotor = pros::Motor(Constants::LEFT_REAR_MOTOR_PORT);
	pros::Motor rightFrontMotor = pros::Motor(Constants::RIGHT_FRONT_MOTOR_PORT);
	pros::Motor rightRearMotor = pros::Motor(Constants::RIGHT_REAR_MOTOR_PORT);

	//singleton instance
  static Drive* instance;

  int outputs[4];
  DriveState state;
	static bool fieldCentric;
	static LatchedBoolean fieldCentricToggle;

  Drive();

  static double smoothRotation(int r);

	//field centric should simply need the current angle subtracted (?)
  static double getWheelOutput(PolarPoint stick, double angleOffset, bool isManual);

	//converts polar wheel output to output on the xy-plane
  static Point getWheelVector(double speed, double rotation);

	//generate outputs for x-drive
  static void generateOutputs(int x, int y, int r, int outputs[], bool isManual);
};
#endif
