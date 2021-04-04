#include "MowerMotorSystem.h"

void MowerMotorSystem::Setup(BatterySystem& batterySystem, CurrentSensor& currentSensor, Motor& mowerMotor){
    this->batterySystem = &batterySystem;
    this->currentSensor = &currentSensor;
    this->mowerMotor = &mowerMotor;

    this->mowerMotor->Brake();
}

bool MowerMotorSystem::IsInHightGrass(){

}

bool MowerMotorSystem::IsMowerActive(){
     return this->mowerMotor->GetTargetSpeed() > 0;
}

  bool MowerMotorSystem::IsMowerReady(){
      return this->mowerMotor->GetTargetSpeed() == this->mowerMotor->GetSpeed();
  }

void MowerMotorSystem::SetMowerActive(bool active){
    if(active)
        this->mowerMotor->ChangeSpeed(mowerMotor->GetMaxSpeed() * direction);
    else
        this->mowerMotor->ChangeSpeed(0);
}

void MowerMotorSystem::EmergencyStop(){
    SetMowerActive(false);
    this->mowerMotor->Brake();
}

bool MowerMotorSystem::SetDirection(MowerMotorDirection direction){
    this->direction = direction;
    if(mowerMotor->GetTargetSpeed() != 0){
        SetMowerActive(true);
    }
}

unsigned int MowerMotorSystem::GetMowerAmper(){
    return currentSensor->GetCurrent();
}

bool MowerMotorSystem::ChangeDirection(){
    if(direction == MowerMotor_CW)
        SetDirection(MowerMotor_CCW);
    else
        SetDirection(MowerMotor_CW);

}