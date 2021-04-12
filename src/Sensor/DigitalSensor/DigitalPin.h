#pragma once
#include "DigitalSensor.h"
#include <arduino.h>


class DigitalPin : public DigitalSensor
{
    private:
        byte pin = 0;
        bool reverse = false;
        bool active = false;
        unsigned long activeTimer = 0;
        unsigned int timeToActive = 50;

    public:
        void Setup(byte pin, unsigned int timeToActive,  bool activeOn);
        bool IsActive();
        void Update(uint32_t now);
};

