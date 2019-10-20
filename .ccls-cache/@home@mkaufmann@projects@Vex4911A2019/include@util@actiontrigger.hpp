#ifndef ACTION_TRIGGER_HPP
#define ACTION_TRIGGER_HPP
#include <functional>
class ActionTrigger{
public:

	//default constructor
	ActionTrigger();

	ActionTrigger(std::function<bool()> trigger);

	//check whether trigger is activated
	bool isTriggered();
private:
	//lambda function for determining trigger activation
	std::function<bool()> triggerActivated;
};
#endif
