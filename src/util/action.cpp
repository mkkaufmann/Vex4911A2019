#include "action.hpp"
#include <functional>

Action::Action(){
  actionMethod = [](){};
  finishedMethod = []()->bool{return true;};
  firstRun = true;
}

Action::Action(std::function<void()> _actionMethod, std::function<bool()> finished){
  actionMethod = _actionMethod;
  finishedMethod = finished;
  firstRun = true;
}

void Action::run(){
  if(!isFinished() || firstRun){
    //std::cout << isFinished();
    actionMethod();
    firstRun = false;
  }
}

bool Action::isFinished(){
  return finishedMethod() && !firstRun;
}
