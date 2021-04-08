#pragma once
#include "MotorDriver.h"
#include <Arduino.h>
#include <Ramp.h>

class bts7960 : public MotorDriver
{
    protected:
        int speed = 0;
        int tmpSpeed = 0;

        byte pinA = -1;
        byte pinB = -1;
        byte pinPWMA = -1;
        byte pinPWMB = -1;

        byte currentPinPWM = -1;

        bool reversed = false;

        void changeDirection();
        void setPinsForSpin();

    public:
        
        void Setup(byte pinA, byte pinB, byte pwmPinA, byte pwmPinB);
        void Update(uint32_t now);
        void ChangeSpeed(const int speed);

        void Brake();
        int GetSpeed();

        void Enable(bool enable);
};