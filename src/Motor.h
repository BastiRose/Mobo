#pragma once
#include <Ramp.h>
#include <Arduino.h>
#include "Component.h"

class CurrentSensor;
class VoltageSensor;
class MotorDriver;

class Motor : public Component
{
private:
    MotorDriver* motorDriver = NULL;

    Ramp ramp;

    int maxSpeed = 0;
    unsigned int maxMilliAmp = 0;

public:
    void Setup(MotorDriver& motorDriver, int maxSpeed, unsigned int rampTime, unsigned int maxMilliAmp);

    void Update(uint32_t now);
    void ChangeSpeed(const int speed);
    void ChangeSpeed(const int speed, unsigned long time);
    void Brake();
    int GetSpeed();
    int GetTargetSpeed();
    bool IsBraking();

    unsigned int GetMaxMilliAmp();
    int GetMaxSpeed();

    void Enable(bool enable);

};

