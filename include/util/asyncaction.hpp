#ifndef ASYNC_ACTION_HPP
#define ASYNC_ACTION_HPP
#include "action.hpp"
#include "actiontrigger.hpp"
class AsyncAction{
public:
	//default constructor
	AsyncAction();

	//add trigger to action
	AsyncAction* hasTrigger(ActionTrigger _trigger);
	//add avtion
	AsyncAction* hasAction(Action _action);
	//tun yhr action once triggered
	void run();

	bool isFinished();
private:
	//TODO: implement safeguards making sure tese are initialized
	bool triggerInitialized;
	bool actionInitialized;

	ActionTrigger trigger;
	Action action;
};
#endif
