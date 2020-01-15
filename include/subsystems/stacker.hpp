#ifndef STACKER_HPP
#define STACKER_HPP
#include "main.h"
#include "constants.hpp"
#include "util/latchedboolean.hpp"
#include <functional>
//stacker subsystem
class Stacker{
public:
  //return singleton
  static Stacker* getInstance();

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

  enum StackerState{
    NEUTRAL,
    MANUAL,
    IN,
    OUT
  };

private:

  int output;

  okapi::Motor stacker1Motor = okapi::Motor(Constants::STACKER_TREAD_1_MOTOR_PORT);
  okapi::Motor stacker2Motor = okapi::Motor(Constants::STACKER_TREAD_2_MOTOR_PORT);

  Stacker();

  static Stacker* instance;

  StackerState state = NEUTRAL;

  LatchedBoolean intakeToggle;

  LatchedBoolean outtakeToggle;

};
#endif
