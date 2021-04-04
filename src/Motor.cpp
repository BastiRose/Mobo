#include "Motor.h"
#include <Ramp.h>
#include "Driver/MotorDriver.h"
#include "Sensor/Current/CurrentSensor.h"
#include "Sensor/Voltage/VoltageSensor.h"


void Motor::Setup(MotorDriver& motorDriver, int maxSpeed, unsigned int rampTime, unsigned int maxMilliAmp){
    this->motorDriver = &motorDriver;

    this->maxMilliAmp = maxMilliAmp;
    this->maxSpeed = maxSpeed;

    this->ramp.Setup(-maxSpeed, maxSpeed, rampTime, 0);
    this->ramp.SetValue(0);
}

void Motor::Update(){
    Component::Update();
    
    if(motorDriver->GetSpeed() != ramp.GetTarget())
        motorDriver->ChangeSpeed(ramp.GetValue());
}

void Motor::ChangeSpeed(const int speed){
    if(speed == ramp.GetTarget())
        return;

    if(speed > maxSpeed){
        ramp.RampTo(maxSpeed);
        return;
    }

    if(speed < -maxSpeed){
       ramp.RampTo(-maxSpeed);
       return;
    }
    
    ramp.RampTo(speed);
}

void Motor::ChangeSpeed(const int speed, unsigned long time){
    if(speed == ramp.GetTarget())
        return;

    if(speed > maxSpeed){
        ramp.RampTo(maxSpeed, time);
        return;
    }

    if(speed < -maxSpeed){
       ramp.RampTo(-maxSpeed, time);
       return;
    }

    ramp.RampTo(speed, time);
}

void Motor::Brake(){
    ramp.SetValue(0);
    motorDriver->Brake();
}

int Motor::GetSpeed(){
    return motorDriver->GetSpeed();
}

int Motor::GetTargetSpeed(){
    return ramp.GetTarget();
}

bool Motor::IsBraking(){
    return motorDriver->IsBraking();
}

unsigned int Motor::GetMaxMilliAmp(){
    return maxMilliAmp;
}

int Motor::GetMaxSpeed(){
    return maxSpeed;
}

void Motor::Enable(bool enable){
    motorDriver->Enable(enable);
}