#include "rotationseeker.hpp"

RotationSeeker::RotationSeeker(double target, double p, double i, double d,
double ir, double e, double startingError): targetRotation(target),
 running(true), pid(p, i, d, ir, e, startingError){

}

void RotationSeeker::start(){
  running = true;
}

//returns speed of rotation from PID when passed current Rotation in RAD
int RotationSeeker::getRotationSpeed(double currentRotation){
  if(running){
    return std::min(std::max(std::floor(pid.getOutput(currentRotation)), -127.0), 127.0);
  }else{
    return 0;
  }
}

void RotationSeeker::end(){
  running = false;
}
