#pragma once
#include "main.h"

class DelayedBoolean{
private:
  bool lastValue;
  double transitionTimestamp;
  double delay;
public:
  DelayedBoolean(double timestamp, double _delay);

  bool update(double timestamp, bool value);
};
