#ifndef PATHFOLLOWER_HPP
#define PATHFOLLOWER_HPP
#include "util/point.hpp"
#include "subsystems/positiontracker.hpp"
#include "pid.hpp"
#include "util/util.hpp"
#include "subsystems/drive.hpp"
#include "math.h"
//manages path following
class PathFollower{
public:
  PathFollower(Point t, double r){
    target = t;
    rotTarget = r;
  };
  //runs PID calculations and updates drivetrain, returns whether complete
  //TODO: optimized rotation
  void run(){
    if(complete){
      return;
    }
    //get position and rotation
    lastRecordedPosition = PositionTracker::getGlobalPosition();
    lastRecordedOrientation = fmod(PositionTracker::getTheta(), M_2_PI);
    //if it has settled at target, increment the settle counter
    if(
      Util::epsilonEquals(lastRecordedPosition.getX(), target.getX(), xPID.getEpsilon()) &&
      Util::epsilonEquals(lastRecordedPosition.getY(), target.getY(), yPID.getEpsilon()) &&
      Util::epsilonEquals(lastRecordedOrientation, fmod(rotTarget, M_2_PI), tPID.getEpsilon())){
        settledIterations++;
    }
    //calculate speeds from PID
    //restrain to voltage range
    xSpeed = std::max(std::min((int)(xPID.getOutput(target.getX() - lastRecordedPosition.getX())), 127), -127);
    ySpeed =  std::max(std::min((int)(xPID.getOutput(target.getY() - lastRecordedPosition.getY())), 127), -127);
    tSpeed = std::max(std::min((int)(xPID.getOutput(fmod(rotTarget, M_2_PI) - lastRecordedOrientation)), 127), -127);
    //update drivetrain
    //TODO: input as voltage
    (*Drive::getInstance()).drivePath(xSpeed, ySpeed, tSpeed);
    if(settledIterations >= iterationsForSettle){
      complete = true;
    }
  }
  bool isComplete(){
    return complete;
  }

private:
  Point target = Point(0,0);
  double rotTarget = 0;
  Point lastRecordedPosition = Point(0,0);
  double lastRecordedOrientation = 0;
  PID xPID = PID(0,0,0, 20, 0.25, 0);
  PID yPID = PID(0,0,0, 20, 0.25, 0);
  PID tPID = PID(0,0,0, M_PI_4, M_PI_4/10, 0);
  double xSpeed = 0;
  double ySpeed = 0;
  double tSpeed = 0;
  bool complete;
  double epsilon = 0.3;
  double settledIterations = 0;
  static double iterationsForSettle;
};
#endif
