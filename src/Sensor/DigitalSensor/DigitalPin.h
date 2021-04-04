#pragma once
#include "DigitalSensor.h"
#include <arduino.h>


class DigitalPin : public DigitalSensor
{
    private:
        byte pin = 0;
        bool reverse = false;

    public:
        void Setup(byte pin, bool activeOn);
        bool IsActive();
        void Update();
};

