#include "subsystems/tilter2.hpp"
#include "lib7842/api.hpp"

using namespace lib7842;

okapi::Motor tilterMotor = okapi::Motor(Constants::TILTER_MOTOR_PORT);
const double MIDDLE_HEIGHT {300};
const double UP_HEIGHT {630};
std::shared_ptr<okapi::AsyncPositionController<double, double>> tilterPID = 
okapi::AsyncPosControllerBuilder()
	.withMotor(tilterMotor)
	.withGains(okapi::IterativePosPIDController::Gains{0.001,0.0,0.0001})
	.withGearset(okapi::AbstractMotor::GearsetRatioPair(okapi::AbstractMotor::gearset::red, 7))
	.build();

void initializeTilter(){
	tilterPID->tarePosition();
};

void setTilterDown(){
	tilterPID->setTarget(0);
}

void setTilterUp(){
	tilterPID->setTarget(UP_HEIGHT);
}

void setTilterMiddle(){
	tilterPID->setTarget(MIDDLE_HEIGHT);
}
