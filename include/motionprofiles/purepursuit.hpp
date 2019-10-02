#ifndef PURE_PURSUIT_HPP
#define PURE_PURSUIT_HPP
#include <vector>
#include "waypoint.hpp"
#include "drivesignal.hpp"
class PurePursuit{
public:
	PurePursuit();
	&PurePursuit addWaypoint(Waypoint waypoint);
	&PurePursuit generateRoughPath();
	&PurePursuit smoothPath();
	&PurePursuit finalizePath();
	vector<Waypoint> getPath();
	DriveSignal getDriveOutput(Point position, double theta);
private:
	vector<Waypoint> path;
	void interpolatePoints();
	Waypoint getClosestWaypoint();		
	Waypoint getLookaheadWaypoint();		
};
#endif
