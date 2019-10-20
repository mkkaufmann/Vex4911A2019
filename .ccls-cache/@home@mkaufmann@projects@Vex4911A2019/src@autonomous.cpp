#include "main.h"
#include "subsystems/drive.hpp"
#include "subsystems/subsystemmanager.hpp"
#include "motionprofiles/pathfollower.hpp"
#include "util/point.hpp"
#include "util/autontimer.hpp"
#include "util/action.hpp"
#include "util/actions.hpp"
#include "util/asyncaction.hpp"
#include "util/actiontrigger.hpp"
#include "util/asyncactionfactory.hpp"
#include "subsystems/stacker.hpp"
#include "util/triggers.hpp"
#include "main.h"
#include <memory>
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
  // PositionTracker tracker = *PositionTracker::getInstance();
  // Subsystem* subsystems[] = {&drive, &tracker};
  // SubsystemManager subsystemManager = SubsystemManager(subsystems);
  //we'll see if pathfollower needs to be a subsystem
  // PathFollower testPathFollower = PathFollower(Point(10,10), M_PI_2);

  Drive drive = *Drive::getInstance();
  PositionTracker tracker = *PositionTracker::getInstance();
  Stacker stacker = *Stacker::getInstance();
  //okapi::Logger::initialize(std::make_unique<okapi::Timer>(), "/usd/debug.txt", okapi::Logger::LogLevel::debug);
  double startTime = 0;
  double driveEndTime = startTime + 3;//3
  double endTime = 15;

  //runs all subsystems for 15 seconds (or regular autonomous)
  AsyncAction manageSubsystems =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      Triggers::trueTrigger()))->hasAction(Action(
      Actions::parallelAction(
        stacker.inAction(), Actions::parallelAction(
          stacker.outAction(), Actions::parallelAction(
            drive.inAction(), Actions::parallelAction(
              drive.outAction(), Actions::parallelAction(
				stacker.getIntakeAction(), Actions::parallelAction(
				  tracker.inAction(),
                    tracker.outAction())))))),
      AutonTimer::timeHasPassedTrigger(15)));

  AsyncAction driveFor3Seconds =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      Triggers::trueTrigger()))->hasAction(Action(
      drive.driveManuallyAction(0, 100, 0, true),
        PositionTracker::isNearPointTrigger(0, 10, 1)));


  AsyncAction driveToPoint =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      Triggers::trueTrigger()))->hasAction(Action(
      drive.driveTowardsPointAction(20, 20),
        PositionTracker::isNearPointTrigger(20, 20, 1)));

  AsyncAction backDriving =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      (Triggers::compoundOrTrigger(AutonTimer::timeHasPassedTrigger(3), PositionTracker::isNearPointTrigger(0,10,1)))))->hasAction(Action(
      drive.driveManuallyAction(0, -127, 0, true),
      AutonTimer::timeHasPassedTrigger(6)));

  
  AsyncAction stopDriving =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      (Triggers::compoundOrTrigger(AutonTimer::timeHasPassedTrigger(6), PositionTracker::isNearPointTrigger(0,10,1)))))->hasAction(Action(
      drive.driveManuallyAction(0, 0, 0, true),
      AutonTimer::timeHasPassedTrigger(15)));

  AsyncAction startIntaking =
  *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
      AutonTimer::timeHasPassedTrigger(driveEndTime)))->hasAction(Action(
      stacker.getIntakeAction(),
      Triggers::trueTrigger()));

		/*
		 * start intake
		 * drive forward and seek forward orientation 2 ft
		 * seek facing and drive towards scoring zone
		 * move the arms out?
		 * tilt up
		 * back up 1 ft
		 * 
		 *
		 *
		 * AsyncAction startIntake =
		 * *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(
		 *	Triggers::trueTrigger()))->hasAction(Action(
		 *		Stacker::UNDEFINED_INTAKE_ACTION(),
		 *		Triggers::timeHasPassedTrigger(15)));
		 *
		 *
		 *
		 *
		 *
		 *
		 *
		 *
		 */
    std::vector<AsyncAction> actions;
    actions.push_back(manageSubsystems);
    actions.push_back(driveToPoint);
    actions.push_back(backDriving);
    actions.push_back(stopDriving);
    actions.push_back(startIntaking);
    std::vector<AsyncAction>::iterator it;
  AutonTimer::start();
	while (true) {
    if(AutonTimer::fifteenSecondsPassed()){
	//	okapi::Logger::instance()->debug("fifteen seconds");
	}
    for(auto &it : actions){
     			it.run();
    }

		pros::delay(20);
	}
}
