#include "MowerMotorSystem.h"

void MowerMotorSystem::Setup(BatterySystem& batterySystem, CurrentSensor& currentSensor, Motor& mowerMotor){
    this->batterySystem = &batterySystem;
    this->currentSensor = &currentSensor;
    this->mowerMotor = &mowerMotor;

    this->mowerMotor->Brake();

    if(random(0,2) == 0){
        SetDirection(MowerMotor_CCW);
    } else {
        SetDirection(MowerMotor_CW);
    }
}

void MowerMotorSystem::Update(uint32_t now){
    if(now - beepStart >= beepTime && !IsMowerActive()) {
        mowerMotor->ChangeSpeed(0);
    }
}

bool MowerMotorSystem::IsInHightGrass(){

}

bool MowerMotorSystem::IsMowerActive(){
     return abs(this->mowerMotor->GetTargetSpeed()) == this->mowerMotor->GetMaxSpeed();
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

void MowerMotorSystem::ChangeDirection(){
    if(direction == MowerMotor_CW)
        SetDirection(MowerMotor_CCW);
    else
        SetDirection(MowerMotor_CW);

}

MowerMotorDirection MowerMotorSystem::GetDirection(){
    return direction;
}

void MowerMotorSystem::Beep(unsigned int time){
    if(!IsMowerActive()){
        beepTime = time;
        this->mowerMotor->ChangeSpeed(10);
        beepStart = millis();
    }
}