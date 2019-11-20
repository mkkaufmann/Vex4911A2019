#ifndef TILTER_HPP
#define TILTER_HPP
#include "subsystem.hpp"
#include "constants.hpp"
#include "main.h"
#include <functional>
class Tilter : public Subsystem{
public:
  static Tilter* getInstance();

  void stop();

  void in();

  std::function<void()> inAction();

  void out();

  std::function<void()> outAction();

  void shiftUp();

  std::function<void()> shiftUpAction;

  void shiftDown();
  
  void setMiddle();

  void setUp();
  void setDown();

  std::function<void()> shiftDownAction;

  void adjustThrottle(double output);

  void offsetForward();
  void offsetBackward();

  enum TilterState {
    DOWN,
    MID,
    UP,
  };

  TilterState getState();
private:
  static const int DOWN_ENC;
  static const int MID_ENC;
  static const int UP_ENC;
  static const int OFFSET_INC;
  int offset;
  bool isAdjusting;
  int adjustOutput;

  pros::Motor tilterMotor = pros::Motor(Constants::TILTER_MOTOR_PORT);
  static Tilter* instance;

  TilterState state;

  Tilter();

};
#endif
