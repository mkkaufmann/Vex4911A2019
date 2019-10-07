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


  enum TilterState {
    DOWN,
    DOWN_TO_MID,
    MID,
    MID_TO_UP,
    UP
  };
private:
  pros::Motor tilterMotor = pros::Motor(Constants::TILTER_MOTOR_PORT);

  static Tilter* instance;
};
#endif
