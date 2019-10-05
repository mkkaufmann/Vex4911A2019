#include "autontimer.hpp"

void AutonTimer::start(){
  startTime = pros::millis();
}

int AutonTimer::getCurrentTimeMillis(){
  return pros::millis();
}

int AutonTimer::getTimeSinceStartMillis(){
  if(startTime < 0){
    start();
  }
  return pros::millis() - startTime;
}

double AutonTimer::getTimeSinceStartSeconds(){
  return getTimeSinceStartMillis()/1000.0;
}

std::function<bool()> AutonTimer::timeHasPassedTrigger(double seconds){
  return [&]()->bool{
    return getTimeSinceStartSeconds() - seconds > 0;
  };
}
