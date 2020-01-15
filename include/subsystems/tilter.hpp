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

  void out();

  void shiftUp();

  void shiftDown();
  
  void setMiddle();

  void setUp();
  void setDown();

  void adjustThrottle(double output);

  void offsetForward();
  void offsetBackward();

  int getTrayAngle();

  enum TilterState {
    DOWN,
    MID,
    UP,
  };

  okapi::Potentiometer trayAngle;
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
