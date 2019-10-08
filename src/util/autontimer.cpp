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

bool AutonTimer::timeHasPassed(double seconds){
  std::cout << seconds << std::endl;
  std::cout << AutonTimer::getTimeSinceStartSeconds() - seconds << std::endl;
  return AutonTimer::getTimeSinceStartSeconds() > seconds;
}

std::function<bool()> AutonTimer::timeHasPassedTrigger(double seconds){
  return [=]()->bool{
    return timeHasPassed(seconds);
  };
}

std::function<bool()> AutonTimer::fifteenSecondsPassed = []()->bool{
  return timeHasPassed(15);
};
