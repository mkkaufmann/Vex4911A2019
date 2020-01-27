#ifndef PID_HPP
#define PID_HPP
#include "math.h"
// manages PID
class PID {
public:
  PID(double p, double i, double d, double ir, double e, double startingError) {
    kP = p;
    kI = i;
    kD = d;
    error = 0;
    previousError = startingError;
    integral = 0;
    // restrains the integral to a maximum value
    integralRestraint = ir;
    epsilon = e;
  };
  // runs PID
  double getOutput(double newError) {
    updateError(newError);
    integral += error;

    if (error == 0) {
      integral = 0;
    }
    if (std::abs(error) > integralRestraint) {
      integral = 0;
    }
    double derivative = error - previousError;

    return kP * error + kI * integral + kD * derivative;
  };
  double getEpsilon() { return epsilon; }

private:
  double kP;
  double kI;
  double kD;
  double error;
  double previousError;
  double integral;
  double integralRestraint;
  double epsilon;
  void updateError(double newError) {
    previousError = error;
    error = newError;
  };
};
#endif
