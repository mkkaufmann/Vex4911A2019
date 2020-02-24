#include "subsystems/arm.hpp"

okapi::Motor armMotor = okapi::Motor(Constants::ARM_MOTOR_PORT);
std::shared_ptr<okapi::AsyncPositionController<double, double>> armPID =
    okapi::AsyncPosControllerBuilder()
        .withMotor(armMotor)
        .withGains(okapi::IterativePosPIDController::Gains{0.0032, 0.0, 0.0001})
        .withGearset(okapi::AbstractMotor::GearsetRatioPair(
            okapi::AbstractMotor::gearset::red, 7))
        .build();

void initializeArm() { armPID->tarePosition(); };

void setArmPosition(double position) { armPID->setTarget(position); }

void setArmDown() { armPID->setTarget(0); }

void setArmAlliance() { armPID->setTarget(420); }

void setArmHighMiddle() { armPID->setTarget(460); }

void setArmLowMiddle() { armPID->setTarget(45); }

void setArmManualSpeed(double speed) { armMotor.moveVoltage(12000 * speed); }
