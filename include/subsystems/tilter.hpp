#pragma once
#include "main.h"
#include "subsystem.hpp"

class Tilter : public Subsystem{
public:
	//gets or creates the singleton instance and returns it
	static Tilter& getInstance();

	//manages inputs
	void in();

	//manages outputs
	void out();

	//moves the tilter up in auton (blocks code progression)
	void autoUp();

	//moves the tilter down in auton (does not block code progression)
	void autoDown();
	
	//sets the tilter state to up in opcontrol
	void up();

	//sets the tilter state to down in opcontrol
	void down();

	//offsets the target encoder positions for calibration
	void offsetForward();
	void offsetBackward();

	//returns the potentiometer value
	int getTrayAngle();
	
	//possible states of the tilter
	enum Position{
		UP,
		DOWN
	};
private:
	//tracks whether a singleton instance has been created
	static bool hasInstance;
	
	//singleton instance
	static Tilter instance;

	//motor that runs the tilter
	okapi::Motor tilter;

	//potentiometer used for calibration
	okapi::Potentiometer trayAngle;

	//offset for skipping/calibration
	int offset;

	//state of the tilter
	Position position;

	//constructor is private for singleton
	Tilter();

	//encoder value for the tray to be up
	static const int UP_ENC;

	//encoder value for the tray to be down
	static const int DOWN_ENC;

	//encoder ticks to offset by in one increment
	// + is forward
	static const int OFFSET_INC;

	//potentiometer value at the top
	static const int UP_POT;
};
