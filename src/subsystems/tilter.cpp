#include "tilter.hpp"
#include "util/util.hpp"

Tilter* Tilter::getInstance(){
  if(instance == NULL){
    instance = new Tilter();
  }
  return instance;
}

void Tilter::stop(){
  state = TO_DOWN;
}

void Tilter::in(){
  switch(state){
    case TO_DOWN:
      if(Util::epsilonEquals(tilterMotor.get_position(), DOWN_ENC, 20)){
        state = DOWN;
      }
      break;
    case TO_UP:
      if(Util::epsilonEquals(tilterMotor.get_position(), UP_ENC, 20)){
        state = UP;
      }
      break;
    case TO_MID:
      if(Util::epsilonEquals(tilterMotor.get_position(), MID_ENC, 20)){
        state = MID;
      }
      break;
    default:
      //sitting at position
      break;
  }
}

std::function<void()> Tilter::inAction(){
  return [&]()->void{
    in();
  };
}

void Tilter::out(){
  switch(state){
    case TO_DOWN:
      tilterMotor.move_absolute(DOWN_ENC, 127);
      break;
    case TO_MID:
      tilterMotor.move_absolute(MID_ENC, 127);
      break;
    case TO_UP:
      tilterMotor.move_absolute(UP_ENC, 50);
      break;
    default:
      //do nothing
      break;
  }
}

std::function<void()> Tilter::outAction(){
  return [&]()->void{
    out();
  };
}

void Tilter::shiftUp(){
  switch(state){
    case DOWN:
      state = TO_MID;
      break;
    case MID:
      state = TO_UP;
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
    case MID:
      state = TO_DOWN;
      break;
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
  shiftUpAction = [&]()->void{shiftUp();};
  shiftDownAction = [&]()->void{shiftDown();};
}

const int Tilter::DOWN_ENC = 0;
const int Tilter::MID_ENC = 0;//tune
const int Tilter::UP_ENC = 0;//tune
