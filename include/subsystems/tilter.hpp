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

  std::function<void()> shiftDownAction;

  void adjustThrottle(double output);

  enum TilterState {
    DOWN,
    MID,
    UP,
  };

  pros::Motor tilterMotor = pros::Motor(Constants::TILTER_MOTOR_PORT);
  TilterState getState();
private:
  static const int DOWN_ENC;
  static const int MID_ENC;
  static const int UP_ENC;
		bool isAdjusting;
		int adjustOutput;

  static Tilter* instance;

  TilterState state;

  Tilter();

};
#endif
