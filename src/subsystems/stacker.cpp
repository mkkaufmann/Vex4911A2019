#include "subsystems/stacker.hpp"


Stacker* Stacker::getInstance(){
  if(instance == NULL){
    instance = new Stacker();
  }
  return instance;
};


void Stacker::intake(){
  state = IN;
  output = -127;
};


void Stacker::outtake(){
  state = OUT;
  output = 127;
};


void Stacker::manual(int out){
  state = MANUAL;
  output = out;
}


void Stacker::stateChangeRequest(bool intakePressed, bool outtakePressed){
  if(intakeToggle.update(intakePressed)){
    if(state == IN){
      stop();
    }else{
      intake();
    }
  }else if(outtakeToggle.update(outtakePressed)){
    if(state == OUT){
      stop();
    }else{
      outtake();
    }
  }
}


void Stacker::in(){};

std::function<void()> Stacker::inAction(){
  return [this]()->void{
    in();
  };
}

void Stacker::out(){
  stacker1Motor.move(output);
  stacker2Motor.move(output);
  // std::cout << "stacker out" << std::endl;
};

std::function<void()> Stacker::outAction(){
  return [this]()->void{
    out();
  };
}

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
  }
}


int Stacker::getOutput(){
  return output;
};

std::function<void()> Stacker::getIntakeAction(){
  return intakeAction;
};

std::function<void()> Stacker::getOuttakeAction(){
  return outtakeAction;
};

Stacker::Stacker(){
  output = 0;
  intakeToggle = LatchedBoolean();
  outtakeToggle = LatchedBoolean();
  intakeAction = [this]()->void{intake();};
  outtakeAction = [this]()->void{intake();};
  state = NEUTRAL;
};
