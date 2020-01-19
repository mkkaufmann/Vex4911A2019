#include "subsystems/stacker.hpp"


Stacker* Stacker::getInstance(){
  if(instance == NULL){
    instance = new Stacker();
  }
  return instance;
};


void Stacker::intake(){
  state = IN;
  output = -12000;
};


void Stacker::outtake(){
  state = OUT;
  output = 12000;
};


void Stacker::slowOuttake(){
  state = OUT;
  output = 12000 * 0.3;
};

void Stacker::manual(int out){
  state = MANUAL;
  output = out;
}


void Stacker::stateChangeRequest(bool intakePressed, bool outtakePressed){
  if(intakePressed){
    if(state == IN){
      stop();
    }else{
      intake();
    }
  }else if(outtakePressed){//untoggled
    if(state == OUT){
      stop();
    }else{
      outtake();
    }
  }else{
		  stop();
		}
}


void Stacker::in(){};

void Stacker::out(){
};

void Stacker::stop(){
  state = NEUTRAL;
  output = 0;
}


int Stacker::getState(){
  switch(state){
    case MANUAL:
      return 0;
      break;
    case NEUTRAL:
      return 1;
      break;
    case IN:
      return 2;
      break;
    case OUT:
      return 3;
      break;
    default:
	  return -1;
  }
}


int Stacker::getOutput(){
  return output;
};

Stacker::Stacker(){
  output = 0;
  intakeToggle = LatchedBoolean();
  outtakeToggle = LatchedBoolean();
  state = NEUTRAL; 
};

Stacker* Stacker::instance = Stacker::getInstance();
