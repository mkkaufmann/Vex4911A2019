#include "auton.
void sixCubeRed() {

  // full speed to line
  driveToPoint({0_in, 12_in}, 0_deg, 1, 7_in);
  rollerIntake();
  // first cube
  model->setMaxVoltage(8000);
  driveToPoint({0_in, 15_in}, 0_deg, 1, 7_in);
  // second cube
  model->setMaxVoltage(7000);
  driveToPoint({0_in, 20_in}, 0_deg, 1, 7_in);
  // third cube
  model->setMaxVoltage(5300);
  driveToPoint({0_in, 40_in}, 0_deg, 1, 7_in);
  // rest of line
  model->setMaxVoltage(4000);
  driveToPoint({0_in, 48_in});
  // pros::delay(400);
  // tower cube
  //	model->setMaxVoltage(12000);
  //	driveToPoint({0_in, 38_in});
  //	driveToPoint({-13_in, 44_in});
  //	model->setMaxVoltage(7000);
  //	driveToPoint({-13_in, 55_in});
  //	pros::delay(400);
  //	driveToPoint({-13_in, 45_in});
  // tower cube 2
  //			model->setMaxVoltage(12000);
  //			driveToPoint({-5_in, 50_in}, 0_deg, 1, 4_in, 3_deg);
  //			model->setMaxVoltage(4000);
  //			driveToPoint({-5_in, 59_in});
  //			pros::delay(400);
  //			model->setMaxVoltage(12000);
  //			driveToPoint({-5_in, 40_in});
  stackerMotor1->moveRelative(800, 12000);
  stackerMotor2->moveRelative(800, 12000);
  // line up
  model->setMaxVoltage(12000);
  driveToPoint({15_in, 12_in}, 135_deg, 2);
  setTilterMiddle();
  driveToPoint({16_in, 9_in}, 135_deg, 1, 2_in);
  setTilterUp();
  pros::delay(2000);
  rollerOuttake(0.5);
  driveToPoint({0_in, 21_in}, 135_deg, 2, 1_in, 2_deg);
}
