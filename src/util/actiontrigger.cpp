 #include "actiontrigger.hpp"

ActionTrigger::ActionTrigger(){
  triggerActivated = []()->bool{return false;};
}

ActionTrigger::ActionTrigger(std::function<bool()> trigger){
  triggerActivated = trigger;
}
//check whether trigger is activated
bool ActionTrigger::isTriggered(){
  return triggerActivated();
}
