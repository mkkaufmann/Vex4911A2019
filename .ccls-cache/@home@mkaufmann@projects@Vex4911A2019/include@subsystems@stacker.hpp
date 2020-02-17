#ifndef STACKER_HPP
#define STACKER_HPP
#include "constants.hpp"
#include "main.h"
#include "util/latchedboolean.hpp"
#include <functional>
// stacker subsystem
class Stacker {
public:
  // return singleton
  static Stacker *getInstance();

  void intake();

  void outtake();

  void slowOuttake();

  void manual(int out);

  void stateChangeRequest(bool intakePressed, bool outtakePressed);

  void in();

  void out();

  void stop();

  int getState();

  int getOutput();

  enum StackerState { NEUTRAL, MANUAL, IN, OUT };

private:
  int output;

  Stacker();

  static Stacker *instance;

  StackerState state = NEUTRAL;

  LatchedBoolean intakeToggle;

  LatchedBoolean outtakeToggle;
};
#endif
