#pragma once
#include "Sensor/Current/CurrentSensor.h"
#include "BatterySystem.h"
#include "Motor.h"
#include "Component.h"

typedef enum
{
    MowerMotor_CW = 1,
    MowerMotor_CCW = -1
} MowerMotorDirection;

class MowerMotorSystem: public Component {
private:
    BatterySystem* batterySystem;
    CurrentSensor* currentSensor;
    Motor* mowerMotor;
    MowerMotorDirection direction = MowerMotor_CW;

    unsigned long beepStart = 0;
    unsigned int beepTime = 1000;

public:
    void Setup(BatterySystem& batterySystem, CurrentSensor& currentSensor, Motor& mowerMotor);
    void Update(uint32_t now);
    bool IsInHightGrass();

    bool IsMowerActive();
    bool IsMowerReady();
    void SetMowerActive(bool active);
    void EmergencyStop();

    unsigned int GetMowerAmper();

    bool SetDirection(MowerMotorDirection direction);
    void ChangeDirection();
    MowerMotorDirection GetDirection();

    void Beep(unsigned int time);

};

