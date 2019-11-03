#include "subsystems/tilter.hpp"
#include "util/util.hpp"

Tilter* Tilter::getInstance(){
  if(instance == NULL){
    instance = new Tilter();
  }
  return instance;
}

void Tilter::stop(){
  state = DOWN;
}

void Tilter::in(){}

std::function<void()> Tilter::inAction(){
  return [this]()->void{
    in();
  };
}

void Tilter::out(){
  switch(state){
    case DOWN:
      tilterMotor.move_absolute(DOWN_ENC, 127);
      break;
    case UP:
		//macro
		if(tilterMotor.get_position() > MID_ENC){
				tilterMotor.move_absolute(UP_ENC, 127 * 1);
		}else{
				tilterMotor.move_absolute(UP_ENC, 127 * 0.3);
		}
		if(isAdjusting && tilterMotor.get_position() > UP_ENC - 200){
				tilterMotor.move(adjustOutput);
		}
      break;
    default:
      //do nothing
      break;
  }
}

std::function<void()> Tilter::outAction(){
  return [this]()->void{
    out();
  };
}

void Tilter::shiftUp(){
  switch(state){
    case DOWN:
      state = UP;
      break;
    default:
      //TODO: rumble
      break;
  }
}

void Tilter::shiftDown(){
  switch(state){
    case UP:
	  state = DOWN;
    default:
      //rumble
      break;
  }
}

Tilter::TilterState Tilter::getState(){
  return state;
}

void Tilter::adjustThrottle(double output){
		if(output == 0){
				isAdjusting = false;
				return;
		}
		if(state == UP){
				isAdjusting = true;
				adjustOutput = output;
		}else{
				isAdjusting = false;
		}
}

Tilter::Tilter(){
  state = DOWN;
  tilterMotor.tare_position();
  shiftUpAction = [this]()->void{shiftUp();};
  shiftDownAction = [this]()->void{shiftDown();};
}

const int Tilter::DOWN_ENC = 0;
const int Tilter::UP_ENC = -2225;//tune
const int Tilter::MID_ENC = UP_ENC * 0.75;//tune
