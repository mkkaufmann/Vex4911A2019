#include "subsystems/tilter.hpp"
#include "util/util.hpp"

Tilter *Tilter::getInstance() {
  if (instance == NULL) {
    instance = new Tilter();
  }
  return instance;
}

void Tilter::stop() { state = DOWN; }

void Tilter::in() {}

void Tilter::out() {
  switch (state) {
  case DOWN:
    tilterMotor.moveAbsolute(DOWN_ENC + offset, 12000);
    break;
  case UP:
    if (trayAngle.get() < 500) {
      if (trayAngle.get() > 400) {
        tilterMotor.moveAbsolute(UP_ENC + offset, 12000 * 0.5);
        if (tilterMotor.getPosition() < UP_ENC + offset + 100) {
          offsetForward();
        }
      } else {
        tilterMotor.moveAbsolute(UP_ENC + offset, 12000 * 0.8);
      }
    }
    break;
  default:
    // do nothing
    break;
  }
}

// used in teleop
void Tilter::shiftUp() {
  switch (state) {
  case DOWN:
    state = UP;
    break;
  default:
    // TODO: rumble
    break;
  }
}

void Tilter::shiftDown() {
  switch (state) {
  case UP:
    state = DOWN;
  default:
    // rumble
    break;
  }
}

// used in autonomous
void Tilter::setMiddle() {
  tilterMotor.moveAbsolute(UP_ENC * 0.3, 12000);
  state = UP;
}
void Tilter::setUp() {
  while (trayAngle.get() < 700) {
    if (trayAngle.get() > 600) {
      tilterMotor.moveAbsolute(UP_ENC + offset, 127 * 0.35);
    } else {
      tilterMotor.moveAbsolute(UP_ENC + offset, 127 * 0.8);
    }
    if (tilterMotor.getPosition() < UP_ENC + offset + 100) {
      offsetForward();
    }
  }
  state = UP;
}
void Tilter::setDown() {
  tilterMotor.moveAbsolute(DOWN_ENC + offset, 127);
  state = DOWN;
}

Tilter::TilterState Tilter::getState() { return state; }

// used for adjusting in teleop
void Tilter::adjustThrottle(double output) {
  if (output == 0) {
    isAdjusting = false;
    return;
  }
  if (state == UP) {
    isAdjusting = true;
    adjustOutput = output;
  } else {
    isAdjusting = false;
  }
}

// used for offsetting for skipping
void Tilter::offsetForward() { offset += OFFSET_INC; }

void Tilter::offsetBackward() { offset -= OFFSET_INC; }

int Tilter::getTrayAngle() { return trayAngle.get(); }

Tilter::Tilter() : trayAngle(7) {
  state = DOWN;
  tilterMotor.tarePosition();
  offset = 0;
}

// encoder values of the different positions
const int Tilter::DOWN_ENC = 0;
const int Tilter::UP_ENC = 3800;    // tune
const int Tilter::OFFSET_INC = -20; // tune
Tilter *Tilter::instance = Tilter::getInstance();
