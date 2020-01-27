#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP
class Waypoint {
public:
  Waypoint(Point point);
  &Waypoint setDistanceAlongPath(double distance);
  &Waypoint setTargetVelocity(double velocity);
  &Waypoint setCurvature(double curvature);

private:
  double distanceAlongPath;
  double targetVelocity;
  double curvature;
};
#endif
