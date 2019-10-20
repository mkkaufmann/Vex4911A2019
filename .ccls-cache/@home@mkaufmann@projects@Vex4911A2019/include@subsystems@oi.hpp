#ifndef OI_HPP
#define OI_HPP
#include "main.h"
#include "subsystem.hpp"
//class for managing user control
class OI : public Subsystem{
public:
  static OI* getInstance(){
    if(instance == NULL){
      instance = new OI();
    }
    return instance;
  };
  void in(){

  };
  void out(){
  };
private:
  OI(){
  };
  static OI* instance;
  pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
};
#endif
