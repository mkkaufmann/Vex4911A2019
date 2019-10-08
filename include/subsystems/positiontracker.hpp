#ifndef POSITIONTRACKER_HPP
#define POSITIONTRACKER_HPP
#include "util/polarpoint.hpp"
#include "util/point.hpp"
#include "util/util.hpp"
#include "constants.hpp"
#include "util/pointutil.hpp"
#include "subsystem.hpp"
#include "main.h"
#include <functional>
//tracks absolute position of the bot
class PositionTracker : public Subsystem{
public:
  static PositionTracker* getInstance();
  static void updatePosition(double leftEnc, double rightEnc, double backEnc);
  static Point getGlobalPosition();
  void in();
  void out();
  std::function<void()> outAction();
  std::function<void()> inAction();
  double getLEncValue();
  double getREncValue();
  double getBEncValue();
  static double getTheta();
  static double getLoopedTheta();
  //returns whether the bot is located in the rectangle with corners a and b
  static bool isInRegion(Point a, Point b);
  static bool isNearPoint(Point a, double epsilonInches);
  static bool isNearAngle(double target, double epsilonRadians);
  static std::function<bool()> isInRegionTrigger(double x1, double y1, double x2, double y2);
  static std::function<bool()> isNearPointTrigger(double x, double y, double epsilonInches);
  static std::function<bool()> isNearAngleTrigger(double target, double epsilonRadians);

  // void reset(){
  //   leftEncAtLastReset = pLeftEnc;
  //   rightEncAtLastReset = pRightEnc;
  //   thetaAtLastReset = pTheta;
  // }

  // static std::function<bool()> getIsNearRotationTrigger(){
  //
  // }
private:
  static PositionTracker* instance;
  PositionTracker();
  //physical encoders
  pros::ADIEncoder lEnc = pros::ADIEncoder(Constants::LEFT_TRACKING_ENCODER_TOP,
                          Constants::LEFT_TRACKING_ENCODER_BOTTOM);;
  pros::ADIEncoder rEnc = pros::ADIEncoder(Constants::RIGHT_TRACKING_ENCODER_TOP,
                          Constants::RIGHT_TRACKING_ENCODER_BOTTOM);;
  pros::ADIEncoder bEnc = pros::ADIEncoder(Constants::REAR_TRACKING_ENCODER_TOP,
                          Constants::REAR_TRACKING_ENCODER_BOTTOM);;
  static double pLeftEnc;
  static double pRightEnc;
  static double pBackEnc;
  static double pTheta;
  static double leftEncAtLastReset;
  static double rightEncAtLastReset;
  static double thetaAtLastReset;
  static Point globalPosition;
};
#endif
