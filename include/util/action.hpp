#ifndef ACTION_HPP
#define ACTION_HPP
#include <functional>
class Action{
public:
	//default constructior
	Action();

	Action(std::function<void()> _actionMethod, std::function<bool()> finished);

	//runs action if possible
	void run();

	bool isFinished();
private:
	//used for single run functions
	bool firstRun;
	std::function<void()> actionMethod;
	std::function<bool()> finishedMethod;
};
#endif
