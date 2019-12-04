#include "subsystems/tilter.hpp"

Tilter& Tilter::getInstance(){
	return instance;
}

void Tilter::in(){};

void Tilter::out(){
	switch(position){
		case DOWN:{
			tilter.moveAbsolute(DOWN_ENC + offset, 12000);
			break;
		}
		case UP:{
			//if not already at the top position
			if(trayAngle.get() < UP_POT){
				//if close, slow down
				if(trayAngle.get() > UP_POT * 0.75){
					tilter.moveAbsolute(UP_ENC + offset, 12000 * 0.5);
				}else{
					//otherwise, continue at full speed
					tilter.moveAbsolute(UP_ENC + offset, 12000);
				}
				//if close to encoder stop, offset the encoder stop to calibrate
				if(tilter.getPosition() < (UP_ENC + offset) * 0.97){
					offsetForward();
				}
			}
			break;
		}
	}
}

void Tilter::autoUp(){
	//while not already at the top position
	while(trayAngle.get() < UP_POT){
		//if close, slow down
		if(trayAngle.get() > UP_POT * 0.75){
			tilter.moveAbsolute(UP_ENC + offset, 12000 * 0.5);
		}else{
			//otherwise, continue at full speed
			tilter.moveAbsolute(UP_ENC + offset, 12000);
		}
		//if close to encoder stop, offset the encoder stop to calibrate
		if(tilter.getPosition() < (UP_ENC + offset) * 0.97){
			offsetForward();
		}
	}
	position = UP;
}

void Tilter::autoDown(){
	//set to the bottom position
	tilter.moveAbsolute(DOWN_ENC + offset, 12000);
	position = DOWN;
}

void Tilter::up(){
	position = UP;
}

void Tilter::down(){
	position = DOWN;
}

void Tilter::offsetForward(){
	offset += OFFSET_INC;
}

void Tilter::offsetBackward(){
	offset -= OFFSET_INC;
}

int Tilter::getTrayAngle(){
	return trayAngle.get();
}

Tilter Tilter::instance = Tilter::getInstance();

Tilter::Tilter(): 
	tilter(okapi::Motor(7)), 
	trayAngle(okapi::Potentiometer(7)
	){
	offset = 0;
	position = DOWN;
}

const int Tilter::UP_ENC = -4900;
const int Tilter::DOWN_ENC = 0;
const int Tilter::OFFSET_INC = -50;
const int Tilter::UP_POT = 670;
