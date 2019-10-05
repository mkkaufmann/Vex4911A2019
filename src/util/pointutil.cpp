#include "util/pointutil.hpp"

//see pointutil.hpp

void PointUtil::print(Point p){
  std::cout << "(" << p.getX() << ", " << p.getY() << ")\n";
};

void PointUtil::print(PolarPoint p){
  std::cout << "(" << p.getRadius() << ", " << Util::toDegrees(p.getAngle()) << ")\n";
};

PolarPoint PointUtil::toPolar(Point other){
  double x = other.getX();
  double y = other.getY();
  if(x == 0){
    if(y > 0){
      return PolarPoint(y, M_PI_2);
    }else if(y < 0){
      return PolarPoint(std::abs(y), -M_PI_2);
    }else{
      return PolarPoint(0, 0);
    }
  }else{
    return PolarPoint(std::sqrt(x * x + y * y), std::atan2(y, x));
  }
};

Point PointUtil::toCartesian(PolarPoint p){
  return Point(p.getRadius() * std::cos(p.getAngle()), p.getRadius() * std::sin(p.getAngle()));
};

double PointUtil::distance(Point a, Point b){
  return std::sqrt((a.getX() - b.getX())*(a.getX() - b.getX()) + (a.getY() - b.getY())*(a.getY() - b.getY()));
}
