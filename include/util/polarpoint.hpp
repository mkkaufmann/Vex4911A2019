#ifndef POLARPOINT_HPP
#define POLARPOINT_HPP
#include "util.hpp"
#include <math.h>
// model for a polar point
class PolarPoint {
public:
  // constructor
  PolarPoint(double r, double theta);

  // divide radius by a scalar
  void divideBy(double num);

  // getter
  double getRadius();

  // limits radius to 1 or less
  void limitRadius();

  // offsets angle
  void rotateAngle(double theta);

  // getter
  double getAngle();

private:
  double radius;
  double angle;
};
#endif
