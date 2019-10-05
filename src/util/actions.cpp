#include "actions.hpp"

std::function<void()> Actions::nothingAction(){
  return []()->void{
    //do nothing
  };
}

std::function<void()> Actions::parallelAction(std::function<void ()> a, std::function<void ()> b){
  return [&]()->void{
    a();
    b();
  };
}
