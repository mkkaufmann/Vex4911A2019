#include "action.hpp"
#include <functional>
#include <iostream>

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
  // if(isFinished()){
  //   // std::cout << "finished" << std::endl;
  // }else{
  //   std::cout << "not finished" << std::endl;
  // }
  if(!isFinished() || firstRun){
    // std::cout << "inside action run" << std::endl;
    actionMethod();
    firstRun = false;
  }
}

bool Action::isFinished(){
  return finishedMethod() && !firstRun;
}
