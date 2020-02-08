#include "util/timedelayedboolean.hpp"

DelayedBoolean::DelayedBoolean(double timestamp, double _delay) {
  transitionTimestamp = timestamp;
  lastValue = false;
  delay = _delay;
}

bool DelayedBoolean::update(double timestamp, bool value) {
  bool result = false;

  if (value && !lastValue) {
    transitionTimestamp = timestamp;
  }

  if (value && (timestamp - transitionTimestamp > delay)) {
    result = true;
  }
  lastValue = value;
  return result;
};
