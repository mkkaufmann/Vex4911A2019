#include "subsystems/drive.hpp"

Drive* Drive::getInstance(){
  if(instance == NULL){
    instance = new Drive();
  }
  return instance;
};

void Drive::stop(){
  state = NEUTRAL;
  out();
}

void Drive::drivePath(int x, int y, int r){
  state = PATH_FOLLOWING;
  fieldCentric = true;
  generateOutputs(x, y, r, outputs, false);
}

void Drive::driveManually(int x, int y, int r){
  state = MANUAL;
  generateOutputs(x, y, r, outputs, true);
}

void Drive::drivePseudoManual(int x, int y, int r){
  state = MANUAL;
  generateOutputs(x, y, r, outputs, false);
}

std::function<void()> Drive::driveManuallyAction(int x, int y, int r, bool isFieldCentric){
  return [&]()->void{
    fieldCentric = isFieldCentric;
    drivePseudoManual(x, y, r);
  };
}

std::function<void()> Drive::driveTowardsPointAction(Point a){
  return [&]()->void{
    //target - position, scale to right speed based on distance.
    //a little naive rn
    Point pos = PositionTracker::getGlobalPosition();
    double distanceInches = PointUtil::distance(pos, a);
    double dx = a.getX() - pos.getX();
    double dy = a.getY() - pos.getY();

    double smoothFactor = 127;
    if(distanceInches < 36){
      smoothFactor = std::exp(distanceInches * std::log(128)/36) - 1;
    }

    double d[] = {dx, dy};
    double largest_dmag = Util::largestMagnitude(d, 2);
    int scaled_dx = dx/largest_dmag;
    int scaled_dy = dy/largest_dmag;

    int xSpeed = scaled_dx * smoothFactor;
    int ySpeed = scaled_dy * smoothFactor;
    drivePseudoManual(xSpeed, ySpeed, 0);
  };
}

std::function<void()> Drive::driveTowardsPointAndOrientationAction(Point a, double radians){
  return [&]()->void{
    //target - position, scale to right speed based on distance.
    //a little naive rn
    Point pos = PositionTracker::getGlobalPosition();
    double currentRot = std::fmod(PositionTracker::getTheta(), M_2_PI);
    double distanceInches = PointUtil::distance(pos, a);
    double dx = a.getX() - pos.getX();
    double dy = a.getY() - pos.getY();
    double dr = radians - currentRot;
    //other direction is closer
    if(std::abs(dr) > M_PI){
      dr = std::fmod(radians + M_PI, M_2_PI) - std::fmod(currentRot + M_PI, M_2_PI);
    }

    double smoothFactor = 127;
    if(distanceInches < 36){
      smoothFactor = std::exp(distanceInches * std::log(128)/36) - 1;
    }

    double d[] = {dx, dy};
    double largest_dmag = Util::largestMagnitude(d, 2);
    int scaled_dx = dx/largest_dmag;
    int scaled_dy = dy/largest_dmag;

    int xSpeed = scaled_dx * smoothFactor;
    int ySpeed = scaled_dy * smoothFactor;

    int rSpeed =  -127*std::sin(dr/2);

    fieldCentric = true;
    drivePseudoManual(xSpeed, ySpeed, rSpeed);
  };
}

void Drive::updateFieldCentric(bool pressed){
  if(fieldCentricToggle.update(pressed)){
    fieldCentric = !fieldCentric;
  }
};

void Drive::in() {}

std::function<void()> Drive::inAction(){
  return [&]()->void{
    in();
  };
}

void Drive::out(){
  switch(state){
    case NEUTRAL:
      leftFrontMotor.move(0);
      leftRearMotor.move(0);
      rightFrontMotor.move(0);
      rightRearMotor.move(0);
      break;
    case MANUAL:
      leftFrontMotor.move(outputs[0]);
      leftRearMotor.move(outputs[1]);
      rightFrontMotor.move(outputs[2]);
      rightRearMotor.move(outputs[3]);
      break;
    case PATH_FOLLOWING:
      leftFrontMotor.move(outputs[0]);
      leftRearMotor.move(outputs[1]);
      rightFrontMotor.move(outputs[2]);
      rightRearMotor.move(outputs[3]);
      break;
  }
};

std::function<void()> Drive::outAction(){
  return [&]()->void{
    in();
  };
}

Drive::Drive(): state(NEUTRAL){}

//deprecated for Util::curveJoystick
double Drive::smoothRotation(int r){
  return 0.75*sin((M_PI*r)/254);
};

double Drive::getWheelOutput(PolarPoint stick, double angleOffset, bool isManual){
  /*
    add offset to the joystick to convert to cos when necessary
    add 2PI for proper modulo (maybe unnecessary?)
    subtract PI/4 to offset the joystick so forward on the bot is forward
    (if driving manually, otherwise do not offset)
    on the joystick
    mod by 2PI to restrict the domain

    scale output by radius
  */
  double radians = std::fmod((stick.getAngle() + angleOffset + M_PI_2 + 2 * M_PI - (isManual ? M_PI_4: 0)), (2 * M_PI));
  if(fieldCentric){
    radians -= /*std::fmod(*/PositionTracker::getTheta()/*, M_2_PI)*/;
  }
  //multiply by curved radius if it's manual
  return (!isManual ? stick.getRadius() : Util::convertToPercent(Util::curveJoystick(true, Util::convertToVoltage(stick.getRadius()), 5))) * std::sin(radians);
};

Point Drive::getWheelVector(double speed, double rotation){
  return Point(std::cos(rotation) * speed, std::sin(rotation) * speed);
};



void Drive::generateOutputs(int x, int y, int r, int outputs[], bool isManual){
  //convert cartesian joystick input into polar coordinates
  PolarPoint translationInput = PointUtil::toPolar(Point(Util::convertToPercent(x), Util::convertToPercent(y)));
  //limit to inside the unit circle
  translationInput.limitRadius();

  //rotation input
  //TODO tune values here/extract them
  double rotInput = isManual ? Util::convertToPercent(Util::curveJoystick(true, r, 5)) :Util::convertToPercent(r);


  //create fake joystick values for rotation of each wheel based on the angles of the wheels
  PolarPoint l1Turn = PolarPoint(rotInput, Util::toRadians(45));
  PolarPoint l2Turn = PolarPoint(rotInput, Util::toRadians(135));
  PolarPoint r1Turn = PolarPoint(rotInput, Util::toRadians(135));
  PolarPoint r2Turn = PolarPoint(rotInput, Util::toRadians(45));

  if(rotInput < 0){
    l1Turn = PolarPoint(rotInput, Util::toRadians(225));
    l2Turn = PolarPoint(rotInput, Util::toRadians(315));
    r1Turn = PolarPoint(rotInput, Util::toRadians(315));
    r2Turn = PolarPoint(rotInput, Util::toRadians(225));
  }

  //get translation outputs (PI/2 offset converts from sin to cos)
  double l1Out = getWheelOutput(translationInput, M_PI_2, isManual);
  double l2Out = getWheelOutput(translationInput, 0, isManual);
  //right wheels are opposite direction hence the negative
  double r1Out = -getWheelOutput(translationInput, 0, isManual);
  double r2Out = -getWheelOutput(translationInput, M_PI_2, isManual);

  //generate rotation outputs
  double l1Rot = getWheelOutput(l1Turn, M_PI_2, isManual);
  double l2Rot = getWheelOutput(l2Turn, 0, isManual);
  double r1Rot = getWheelOutput(r1Turn, 0, isManual);
  double r2Rot = getWheelOutput(r2Turn, M_PI_2, isManual);

  //put outputs into array form for easy iteration
  double outArr [] = {l1Out, l2Out, r1Out, r2Out};
  double rotArr [] = {l1Rot, l2Rot, r1Rot, r2Rot};
  if(rotInput < 0){
    for(int i = 0; i < sizeof(rotArr)/8; i++){
      //TODO figure out permanent fix
      rotArr[i] = rotArr[i] * -1;
    }
  }
  //calculate largest outputs and divide each by the largest
  double largestOut = Util::largest(outArr, sizeof(outArr)/8);
  for(int i = 0; i < sizeof(outArr)/8; i++){
    outArr[i] = Util::safeDivide(outArr[i], largestOut);
  }

  double largestRot = Util::largest(rotArr, sizeof(rotArr)/8);
  for(int i = 0; i < sizeof(rotArr)/8; i++){
    //TODO figure out permanent fix
    rotArr[i] = Util::safeDivide(rotArr[i], largestRot * Util::unit(rotInput));
  }

  //calculate the total input and separate priorities
  double totalSum = std::abs(rotInput) + std::abs(translationInput.getRadius());
  double rotPriority = rotInput/totalSum;
  double transPriority = translationInput.getRadius()/totalSum;

  //scale outputs by priority
  for(int i = 0; i < sizeof(outArr)/8; i++){
    outArr[i] = outArr[i] * transPriority * translationInput.getRadius();
  }

  for(int i = 0; i < sizeof(rotArr)/8; i++){
    rotArr[i] = rotArr[i] * rotPriority * rotInput;
  }

  //set outputs to the sum of translation and rotation outputs
  for(int i = 0; i < 4; i++){
    outputs[i] = Util::convertToVoltage(outArr[i] + rotArr[i]);
    pros::lcd::clear_line(1);
  }
};
