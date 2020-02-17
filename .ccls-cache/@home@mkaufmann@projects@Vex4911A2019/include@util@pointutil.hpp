#ifndef POINTUTIL_HPP
#define POINTUTIL_HPP
#include "point.hpp"
#include "polarpoint.hpp"
#include "util.hpp"
#include <iostream>
// utility functions for cartesian and polar points
class PointUtil {
public:
  // prints cartesian point
  static void print(Point p);

  // prints polar point
  static void print(PolarPoint p);

  // converts cartesian point to polar point
  static PolarPoint toPolar(Point other);

  // converts polar point to cartesian point
  static Point toCartesian(PolarPoint p);

  static double distance(Point a, Point b);
};
#endif
