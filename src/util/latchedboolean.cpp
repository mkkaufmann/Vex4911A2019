#include "util/latchedboolean.hpp"

LatchedBoolean::LatchedBoolean() : mLast(false) {}

bool LatchedBoolean::update(bool newValue) {
  bool ret = false;
  if (newValue && !mLast) {
    ret = true;
  }
  mLast = newValue;
  return ret;
}
