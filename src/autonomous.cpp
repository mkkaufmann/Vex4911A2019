#include "main.h"
#include "subsystems/drive.hpp"
#include "subsystems/subsystemmanager.hpp"
#include "motionprofiles/pathfollower.hpp"
#include "util/point.hpp"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  //initialize subsystems and manager
  Drive drive = *Drive::getInstance();
  PositionTracker tracker = *PositionTracker::getInstance();
  Subsystem* subsystems[] = {&drive, &tracker};
  SubsystemManager subsystemManager = SubsystemManager(subsystems);
  //we'll see if pathfollower needs to be a subsystem
  PathFollower testPathFollower = PathFollower(Point(10,10), M_PI_2);
	while (true) {
    if(!testPathFollower.isComplete()){
      testPathFollower.run();
    }
    //run every subsystem
    subsystemManager.cycle();
		pros::delay(20);
	}
}
