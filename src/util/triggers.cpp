#include "triggers.hpp"

std::function<bool()> Triggers::trueTrigger(){
  return []()->bool{return true;};
}

std::function<bool()> Triggers::compoundAndTrigger(std::function<bool()> a, std::function<bool()> b){
  return [&]()->bool{
    return a() && b();
  };
}

std::function<bool()> Triggers::compoundOrTrigger(std::function<bool()> a, std::function<bool()> b){
  return [&]()->bool{
    return a() || b();
  };
}
