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
      tilterMotor.move_absolute(DOWN_ENC + offset, 127);
      break;
    case UP:
		if(trayAngle.get() < 500){
			if(trayAngle.get() > 400){
				tilterMotor.move_absolute(UP_ENC + offset, 127 * 0.5);
			if(tilterMotor.get_position() < UP_ENC + offset + 100){
				offsetForward();
			}
			}else{
				tilterMotor.move_absolute(UP_ENC + offset, 127 * 0.8);
			}
		}
//		//macro
//		if(tilterMotor.get_position() > MID_ENC){
//				tilterMotor.move_absolute(UP_ENC + offset, 127 * 1);
//		}else{
//				tilterMotor.move_absolute(UP_ENC + offset, 127 * 0.5);
//		}
//		if(isAdjusting && tilterMotor.get_position() > UP_ENC + offset - 500){
//				tilterMotor.move(adjustOutput);
//		}
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

//used in teleop
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

//used in autonomous
void Tilter::setMiddle(){
		tilterMotor.move_absolute(UP_ENC * 0.3, 127);
		state = UP;
}
void Tilter::setUp(){
		while(trayAngle.get() < 700){
			if(trayAngle.get() > 600){
				tilterMotor.move_absolute(UP_ENC + offset, 127 * 0.35);
			}else{
				tilterMotor.move_absolute(UP_ENC + offset, 127 * 0.8);
			}
			if(tilterMotor.get_position() < UP_ENC + offset + 100){
				offsetForward();
			}
		}
		state = UP;
}
void Tilter::setDown(){
		tilterMotor.move_absolute(DOWN_ENC + offset, 127);
		state = DOWN;
}

Tilter::TilterState Tilter::getState(){
  return state;
}

//used for adjusting in teleop
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

//used for offsetting for skipping
void Tilter::offsetForward(){
		offset += OFFSET_INC;
}

void Tilter::offsetBackward(){
		offset -= OFFSET_INC;
}

int Tilter::getTrayAngle(){
	return trayAngle.get();
}

Tilter::Tilter():trayAngle(7){
  state = DOWN;
  tilterMotor.tare_position();
  offset = 0;
  shiftUpAction = [this]()->void{shiftUp();};
  shiftDownAction = [this]()->void{shiftDown();};
}

//encoder values of the different positions
const int Tilter::DOWN_ENC = 0;
const int Tilter::UP_ENC = -3800;//tune
const int Tilter::OFFSET_INC = -20;//tune
Tilter* Tilter::instance = Tilter::getInstance();