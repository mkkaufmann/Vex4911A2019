#ifndef ROTATIONSEEKER_HPP
#define ROTATIONSEEKER_HPP
#include "pid.hpp"
#include "math.h"
class RotationSeeker{
public:

  //sets up the target rotation
  RotationSeeker(double target, double p, double i, double d, double ir, double e, double startingError);

  //starts a pid
  void start();

  //updates pid and returns
  int getRotationSpeed(double currentRotation);

  //ends pid
  void end();

private:
    PID pid;
    bool running;
    double targetRotation;
};
#endif
