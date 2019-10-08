#include "actions.hpp"
#include <iostream>
std::function<void()> Actions::nothingAction(){
  return []()->void{
    //do nothing
  };
}

std::function<void()> Actions::parallelAction(std::function<void ()> a, std::function<void ()> b){
  return [=]()->void{
    // std::cout << "parallelAction" << std::endl;
    a();
    b();
  };
}
