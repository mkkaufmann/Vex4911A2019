#include "util/point.hpp"

Point::Point(double ix, double iy) : x(ix), y(iy) {}

Point Point::add(Point other) { return Point(x + other.x, y + other.y); };

void Point::addLocally(Point other) {
  x += other.x;
  y += other.y;
}

double Point::getX() { return x; };

double Point::getY() { return y; };
