#ifndef DRIVE_SIGNAL_HPP
#define DRIVE_SIGNAL_HPP
#include "polarpoint.hpp"
class DriveSignal {
public:
  DriveSignal(PolarPoint driveVector, double rotationSpeed);
  double getRotPower();
  double getDriveVector();
  double getX();
  double getY();

private:
  PolarPoint driveVector;
  double rotSpeed;
}
#endif
