#ifndef UTIL_HPP
#define UTIL_HPP
#include "math.h"
#include "float.h"
//general utility functions
class Util{
public:
  //converts degrees to radians
  static double toRadians(double degrees);

  //convert radians to degrees
  static double toDegrees(double radians);

  //if value is less than 0, returns -1
  //if value if greater than 0, returns 1
  static double unit(double num);

  //finds the largest value in a double array
  static double largest(double arr[], int len);

  //find the value w/ largest magnitude in a double array
  static double largestMagnitude(double arr[], int len);

  //divides a by b, unless b is 0, then return 0
  static double safeDivide(double a, double b);

  //maps x from one range to another
  static double map(double x, double pmin, double pmax, double min, double max);

  //converts percent value [-1, 1] to voltage [-127, 127]
  static int convertToVoltage(double t);

  //converts voltage value [-127, 127] to percent [-1, 1]
  static double convertToPercent(int t);

  //check whether two values are approximately equal
  //https://floating-point-gui.de/errors/comparison/
  static bool epsilonEquals(double a, double b, double epsilon);

  static int curveJoystick(bool red, int input, double t);

  //used when the closest distance between two angles might go through 0
  static double rotateHalfway(double radians);

};
#endif
