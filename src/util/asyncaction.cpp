#include <iostream>
#include <pthread.h>
#include <future>
#include <functional>
#include <vector>
#include "util/action.hpp"
#include "util/actiontrigger.hpp"
#include "util/asyncaction.hpp"

//test subsystem for async actions
/*class TestSubsystem{
public:
	TestSubsystem(int _targetAmount){
		count = 0;
		targetAmount = _targetAmount;
		incrementObjectCount = [](TestSubsystem* object){
			object->count++;
			//std::cout << object->count << std::endl;
		};
		incrementCount = [&](){
			incrementObjectCount(this);
		};
		reachedObjectAmount = [](TestSubsystem* object)->bool{
			//std::cout << object->count << ", " <<  object->targetAmount << ", " << (object->count >= object->targetAmount ) << std::endl;

			return object->count >= object->targetAmount;
		};
		reachedAmount = [&]()->bool{
			return reachedObjectAmount(this);
		};

	}

	std::function<bool()> getCustomAmountTrigger(int amount){
	        return [this, amount]()->bool{
			return this->hasReachedCustomAmount(amount);
		};
	}

	bool hasReachedCustomAmount(int amount){
		return count >= amount;
	}

	int targetAmount;
	int count;
	std::function<bool()> reachedAmount;
	std::function<void()> incrementCount;
private:
	std::function<void(TestSubsystem*)> incrementObjectCount;
	std::function<bool()> reachedCustomAmount();
	std::function<bool(TestSubsystem*)> reachedObjectAmount;
};*/

/*class ActionTrigger{
public:

	//default constructor
	ActionTrigger(){
		triggerActivated = []()->bool{return false;};
	}

	ActionTrigger(std::function<bool()> trigger){
		triggerActivated = trigger;
	}
	//check whether trigger is activated
	bool isTriggered(){
		return triggerActivated();
	}
private:
	//lambda function for determining trigger activation
	std::function<bool()> triggerActivated;
};*/

/*class Action{
public:
	//default constructior
	Action(){
		actionMethod = [](){};
		finishedMethod = []()->bool{return true;};
		firstRun = true;
	}

	Action(std::function<void()> _actionMethod, std::function<bool()> finished){
		actionMethod = _actionMethod;
		finishedMethod = finished;
		firstRun = true;
	}
	//runs action if possible
	void run(){
		if(!isFinished() || firstRun){
			//std::cout << isFinished();
			actionMethod();
			firstRun = false;
		}
	}

	bool isFinished(){
		return finishedMethod() && !firstRun;
	}
private:
	//used for single run functions
	bool firstRun;
	std::function<void()> actionMethod;
	std::function<bool()> finishedMethod;
};*/

// class AsyncAction{
// public:
	//default constructor
	AsyncAction::AsyncAction(){
		triggerInitialized = false;
		actionInitialized = false;
		trigger = ActionTrigger([]()->bool{return true;});
		action = Action();
	}

	//add trigger to action
	AsyncAction* AsyncAction::hasTrigger(ActionTrigger _trigger){
		trigger = _trigger;
		triggerInitialized = true;
		return this;
	}
	//add avtion
	AsyncAction* AsyncAction::hasAction(Action _action){
		action = _action;
		actionInitialized = true;
		return this;
	}
	//tun yhr action once triggered
	void AsyncAction::run(){

	  // std::cout << "asyncAction run" << std::endl;
		if(trigger.isTriggered()){
			// std::cout << "asyncAction triggered" << std::endl;
			action.run();
		}
	}

	bool AsyncAction::isFinished(){
		return action.isFinished();
	}
// private:
// 	//TODO: implement safeguards making sure tese are initialized
// 	bool triggerInitialized;
// 	bool actionInitialized;
//
// 	ActionTrigger trigger;
// 	Action action;
// };

//creates AsyncActions
/*class AsyncActionFactory{
public:
	//creates an async action
	static AsyncAction* makeAsyncAction(){
		AsyncAction* action;
		action = new AsyncAction;
		*action = AsyncAction();
		return action;
	}
private:

};*/

// void called_from_async(){
// 	std::cout << "Async call" << std::endl;
// }

// int main()
// {
// 	TestSubsystem test1 = TestSubsystem(1000);
// 	TestSubsystem test2 = TestSubsystem(20000);
// 	AsyncAction action2 = *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(test1.reachedAmount))->hasAction(Action(test2.incrementCount, test2.reachedAmount));
// 	AsyncAction action1 = *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger([]()->bool{return true;}))->hasAction(Action(test1.incrementCount, test1.reachedAmount));
// 	AsyncAction action3 = *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(test1.reachedAmount))->hasAction(Action([&](){std::cout<<test1.count<<std::endl;}, [](){return true;}));
// 	AsyncAction action4 = *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(test2.reachedAmount))->hasAction(Action([&](){std::cout<<test2.count<<std::endl;}, [](){return true;}));
// 	AsyncAction action5 = *AsyncActionFactory::makeAsyncAction()->hasTrigger(ActionTrigger(test2.getCustomAmountTrigger(500)))->hasAction(Action([&](){std::cout<<test2.count<<std::endl;}, test2.getCustomAmountTrigger(520)));
// 	std::vector<AsyncAction> actions;
// 	actions.push_back(action1);
// 	actions.push_back(action2);
// 	actions.push_back(action3);
// 	actions.push_back(action4);
// 	actions.push_back(action5);
// 	std::vector<AsyncAction>::iterator it;
// 	//std::future<void> result( std::async());
//
// 	//TODO make array of async actions and just look through in this pattern
// 	while(true){
// 		for(auto &it : actions){
// 			it.run();
// 		}
// 	}
// 	return 0;
// }
