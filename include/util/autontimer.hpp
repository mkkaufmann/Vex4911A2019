#ifndef AUTON_TIMER_HPP
#define AUTON_TIMER_HPP
#include "main.h"
#include <functional>
class AutonTimer {
public:
  static void start();

  static int getCurrentTimeMillis();

  static int getTimeSinceStartMillis();

  static double getTimeSinceStartSeconds();

  static bool timeHasPassed(double seconds);

  static std::function<bool()> timeHasPassedTrigger(double seconds);

  static std::function<bool()> fifteenSecondsPassed;

private:
  static int startTime;
};
#endif
