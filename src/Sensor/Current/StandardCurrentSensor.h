#pragma once
#include "CurrentSensor.h"
#include <HighLowPass.h>

class AnalogSensor;

class StandardCurrentSensor : public CurrentSensor
{
    private:
        HighLowPass filter;
        AnalogSensor* analogSensor;
        unsigned long lastUpdate = 0;
        unsigned int updateInterval = 0;
        long factor = 0;

    public:
        void Setup(AnalogSensor& analogSensor, float factor, float filterAlpha, unsigned int updateInterval);
        void Update(uint32_t now);
        unsigned  int GetCurrent();
        unsigned  int GetPower(int voltage);

};



