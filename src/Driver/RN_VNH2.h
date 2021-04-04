#pragma once
#include "MotorDriver.h"
#include <Arduino.h>


class RN_VNH2 : public MotorDriver
{
    protected:
        int speed = 0;
        int tmpSpeed = 0;

        byte pinA = -1;
        byte pinB = -1;
        byte pinPWM = -1;

        bool reversed = false;


        void changeDirection();
        void setPinsForSpin();

    public:
        
        void Setup(byte pinA, byte pinB, byte pwmPin); 

        void Update();

        void ChangeSpeed(const int speed);

        void Brake();

        int GetSpeed();

        void Enable(bool enable);
};

