#pragma once
#include "DigitalSensor.h"
#include <arduino.h>

class AnalogSensor;

class AnalogToDigital : public DigitalSensor
{
    private:
        bool activeHigher = true;
        int value = 0;
        int threshold = 0;
        AnalogSensor* analogSensor;

    public:
        void Setup(AnalogSensor& analogInput, int threshold,  bool activeHigher);
        bool IsActive();
        void Update();
};
