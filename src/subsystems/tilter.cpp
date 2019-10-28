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
    case MID:
      tilterMotor.move_absolute(MID_ENC, 127 * 0.8);
      break;
    case UP:
      tilterMotor.move_absolute(UP_ENC, 127 * 0.4);
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
      //rumble
      break;
  }
}

void Tilter::shiftDown(){
  switch(state){
    case UP:
      //fall through
	  state = DOWN;
    default:
      //rumble
      break;
  }
}

Tilter::TilterState Tilter::getState(){
  return state;
}

Tilter::Tilter(){
  state = DOWN;
  tilterMotor.tare_position();
  shiftUpAction = [this]()->void{shiftUp();};
  shiftDownAction = [this]()->void{shiftDown();};
}

const int Tilter::DOWN_ENC = 0;
const int Tilter::MID_ENC = -2000;//tune
const int Tilter::UP_ENC = -2200;//tune
