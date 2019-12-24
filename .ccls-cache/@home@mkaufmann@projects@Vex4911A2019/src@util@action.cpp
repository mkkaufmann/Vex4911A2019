#include "util/action.hpp"
#include <functional>
#include <iostream>

Action::Action(){
  actionMethod = [](){};
  finishedMethod = []()->bool{return true;};
  firstRun = true;
  hasFinished = false;
}

Action::Action(std::function<void()> _actionMethod, std::function<bool()> finished){
  actionMethod = _actionMethod;
  finishedMethod = finished;
  firstRun = true;
  hasFinished = false;
}

void Action::run(){
  // if(isFinished()){
  //   // std::cout << "finished" << std::endl;
  // }else{
  //   std::cout << "not finished" << std::endl;
  // }
  if(finishedMethod()){
		  hasFinished = true;
  }
  if(!hasFinished || firstRun){
    // std::cout << "inside action run" << std::endl;
    actionMethod();
    firstRun = false;
  }
}

bool Action::isFinished(){
  return hasFinished && !firstRun;
}
