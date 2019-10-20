#ifndef SUBSYSTEMMANAGER_HPP
#define SUBSYSTEMMANAGER_HPP
#include "subsystem.hpp"
//calls functions on all of the subsystems
class SubsystemManager{
public:
  SubsystemManager(Subsystem* allSubsystems[]){
    //hack to find the size of an array
    //https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/
    size = *(&allSubsystems + 1) - allSubsystems;
    //transfer values to class member
    for(int i = 0; i < size; i++){
      allSubsystems[i] = subsystems[i];
    }
  };
  //calls in() of every subsystem
  void in(){
    for(int i = 0; i < size; i++){
      subsystems[i]->in();
    }
  };
  //calls out() of every subsystem
  void out(){
    for(int i = 0; i < size; i++){
      subsystems[i]->out();
    }
  };
  //calls both in and out
  void cycle(){
    in();
    out();
  };
private:
  int size;
  Subsystem* subsystems[];
};
#endif
