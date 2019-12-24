 #include "actiontrigger.hpp"
 #include <iostream>

ActionTrigger::ActionTrigger(){
  triggerActivated = []()->bool{return false;};
}

ActionTrigger::ActionTrigger(std::function<bool()> trigger){
  triggerActivated = trigger;
}
//check whether trigger is activated
bool ActionTrigger::isTriggered(){
  // std::cout << triggerActivated() << std::endl;
  return triggerActivated();
}
