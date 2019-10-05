#ifndef STACKER_HPP
#define STACKER_HPP
#include "subsystem.hpp"
#include "main.h"
#include "constants.hpp"
#include "util/latchedboolean.hpp"
#include <functional>
//stacker subsystem
class Stacker : public Subsystem{
public:
  //return singleton
  static Stacker* getInstance();

  void intake();

  void outtake();

  void manual(int out);

  void stateChangeRequest(bool intakePressed, bool outtakePressed);

  void in();

  std::function<void()> inAction();
  std::function<void()> outAction();

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

  pros::Motor stacker1Motor = pros::Motor(Constants::STACKER_TREAD_1_MOTOR_PORT);
  pros::Motor stacker2Motor = pros::Motor(Constants::STACKER_TREAD_2_MOTOR_PORT);

  Stacker();

  static Stacker* instance;

  StackerState state = NEUTRAL;

  LatchedBoolean intakeToggle;

  LatchedBoolean outtakeToggle;

};
#endif
