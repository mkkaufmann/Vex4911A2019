#include "util/polarpoint.hpp"

//see polarpoint.hpp

PolarPoint::PolarPoint(double r, double theta){
  //ensure positive radius
  if(r < 0){
    radius = -r;
    angle = std::fmod((theta + M_PI) , (2 * M_PI));
  }else{
    radius = r;
    angle = theta;
  }
}

void PolarPoint::divideBy(double num){
  radius = Util::safeDivide(radius, num);
}

double PolarPoint::getRadius(){
  return radius;
}

void PolarPoint::limitRadius(){
  if(radius > 1){
    radius = 1;
  }
}

void PolarPoint::rotateAngle(double theta){
  angle += theta;
}

double PolarPoint::getAngle(){
  return angle;
}
