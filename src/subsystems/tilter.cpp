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
  std::cout << tilterMotor.get_position() << std::endl;
  switch(state){
    case TO_DOWN:
      if(tilterMotor.get_position() > -10 && tilterMotor.get_position() < 10){
        state = DOWN;
      }
      std::cout << "tilter to down" << std::endl;
      break;
    case TO_UP:
      if(tilterMotor.get_position() > UP_ENC){
        state = UP;
      }
      std::cout << "tilter to up" << std::endl;
      break;
    case TO_MID:
      // if(Util::epsilonEquals(tilterMotor.get_position(), MID_ENC, 50)){
      if(tilterMotor.get_position() > MID_ENC){
        state = MID;
      }
      std::cout << "tilter to mid" << std::endl;
      break;
    case DOWN:
      std::cout << "tilter down" << std::endl;
      break;
    case MID:
        std::cout << "tilter mid" << std::endl;
        break;
    case UP:
          std::cout << "tilter up" << std::endl;
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
    case DOWN:
    case TO_DOWN:
      tilterMotor.move_absolute(DOWN_ENC, 40);
      break;
    case MID:
    case TO_MID:
      tilterMotor.move_absolute(MID_ENC, 127);
      break;
    case UP:
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
const int Tilter::MID_ENC = 280;//tune
const int Tilter::UP_ENC = 590;//tune
