#pragma once
#include "Sensor/Current/CurrentSensor.h"
#include "BatterySystem.h"
#include "Motor.h"

typedef enum
{
    MowerMotor_CW = 1,
    MowerMotor_CCW = -1
} MowerMotorDirection;

class MowerMotorSystem {
private:
    BatterySystem* batterySystem;
    CurrentSensor* currentSensor;
    Motor* mowerMotor;
    MowerMotorDirection direction = MowerMotor_CW;

public:
    void Setup(BatterySystem& batterySystem, CurrentSensor& currentSensor, Motor& mowerMotor);
    bool IsInHightGrass();

    bool IsMowerActive();
    bool IsMowerReady();
    void SetMowerActive(bool active);
    void EmergencyStop();

    unsigned int GetMowerAmper();

    bool SetDirection(MowerMotorDirection direction);
    bool ChangeDirection();

};

