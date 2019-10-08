#include "positiontracker.hpp"

PositionTracker* PositionTracker::getInstance(){
  if(instance == NULL){
    instance = new PositionTracker();
  }
  return instance;
}

void PositionTracker::updatePosition(double leftEnc, double rightEnc, double backEnc){

  //change in encoder values
  double dLeftEnc = leftEnc - pLeftEnc;
  double dRightEnc = rightEnc - pRightEnc;
  double dBackEnc = backEnc - pBackEnc;

  //angle orientation of the bot
  double theta = thetaAtLastReset + Util::safeDivide((leftEnc - leftEncAtLastReset) - (rightEnc - rightEncAtLastReset), Constants::LEFT_WHEEL_TO_TRACKING_CENTER + Constants::RIGHT_WHEEL_TO_TRACKING_CENTER);

  //change in angle orientation
  double dTheta = theta - pTheta;

  //arc radii
  double centerXArcRadius = Util::safeDivide(dBackEnc,dTheta) + Constants::REAR_WHEEL_TO_TRACKING_CENTER;
  double centerYArcRadius = Util::safeDivide(dRightEnc, dTheta) + Constants::RIGHT_WHEEL_TO_TRACKING_CENTER;

  //local coordinates
  double dX;
  double dY;
  if(dTheta == 0){
    dX = dBackEnc;
    dY = dRightEnc;
  }else{
    //math
    //http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf
    dX = 2 * std::sin(dTheta/2) * centerXArcRadius;
    dY = 2 * std::sin(dTheta/2) * centerYArcRadius;
  }

  //polar is necessary to rotate from local to global plane
  PolarPoint coordinate = PointUtil::toPolar(Point(dX, dY));

  //converts local to global
  coordinate.rotateAngle(-(pTheta + dTheta/2));
  //global cartesian coordinates
  Point cartesianCoordinate = PointUtil::toCartesian(coordinate);

  //add the coordinates to the running record
  globalPosition.addLocally(cartesianCoordinate);

  //current values become previous values
  pLeftEnc = leftEnc;
  pRightEnc = rightEnc;
  pBackEnc = backEnc;
  pTheta = theta;
}

Point PositionTracker::getGlobalPosition(){
  return globalPosition;
}

void PositionTracker::in(){};

void PositionTracker::out(){
  //double conversion needed to pass value in inches w/ higher accuracy
  updatePosition(lEnc.get_value()/(double)Constants::ENCODER_TICKS_PER_INCH, rEnc.get_value()/(double)Constants::ENCODER_TICKS_PER_INCH, bEnc.get_value()/(double)Constants::ENCODER_TICKS_PER_INCH);
};

std::function<void()> PositionTracker::outAction(){
  return [this]()->void{
    out();
  };
}

std::function<void()> PositionTracker::inAction(){
  return [this]()->void{
    in();
  };
}

double PositionTracker::getLEncValue(){
  return lEnc.get_value();
};

double PositionTracker::getREncValue(){
  return rEnc.get_value();
};

double PositionTracker::getBEncValue(){
  return bEnc.get_value();
};

double PositionTracker::getTheta(){
  return pTheta;
}

double PositionTracker::getLoopedTheta(){
  return std::fmod(getTheta(), M_2_PI);
}

bool PositionTracker::isInRegion(Point a, Point b){
  double minX = std::min(a.getX(), b.getX());
  double minY = std::min(a.getY(), b.getY());
  double maxX = std::max(a.getX(), b.getX());
  double maxY = std::max(a.getY(), b.getY());

  double x = globalPosition.getX();
  double y = globalPosition.getY();
  return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

bool PositionTracker::isNearPoint(Point a, double epsilonInches){
  return PointUtil::distance(a, PositionTracker::getGlobalPosition()) < epsilonInches;
}

bool PositionTracker::isNearAngle(double target, double epsilonRadians){
  //check near in both directions
  return Util::epsilonEquals(PositionTracker::getLoopedTheta(), target, epsilonRadians)
  || Util::epsilonEquals(Util::rotateHalfway(PositionTracker::getLoopedTheta()), Util::rotateHalfway(target), epsilonRadians);
}

std::function<bool()> PositionTracker::isInRegionTrigger(double x1, double y1, double x2, double y2){
  return [=]()->bool{
    Point a = Point(x1, y1);
    Point b = Point(x2, y2);
    return PositionTracker::isInRegion(a, b);
  };
}

std::function<bool()> PositionTracker::isNearPointTrigger(double x, double y, double epsilonInches){
  return [=]()->bool{
    return PositionTracker::isNearPoint(Point(x, y), epsilonInches);
  };
}

std::function<bool()> PositionTracker::isNearAngleTrigger(double target, double epsilonRadians){
  return [=]()->bool{
    return PositionTracker::isNearAngle(target, epsilonRadians);
  };
}

PositionTracker::PositionTracker(){
  //previous values
  pLeftEnc = 0;
  pRightEnc = 0;
  pBackEnc = 0;
  pTheta = 0;
  //values at last reset
  leftEncAtLastReset = 0;
  rightEncAtLastReset = 0;
  thetaAtLastReset = 0;
  //last recorded global position
  globalPosition = Point(0,0);

  lEnc.reset();
  rEnc.reset();
  bEnc.reset();
};
