#include "DistanceSensor.h"
#include <Arduino.h>
#include <limits.h>

class HC_SR04 : public DistanceSensor 
{
private:
    unsigned int measuredDistance = UINT_MAX;

    unsigned long lastMeasurement = 0;

    byte echo = 0;
    byte trigger = 0;
    unsigned int intervall = 0;
    unsigned int timeout = 0;

   

public:
    void Setup(byte echo, byte trigger, unsigned int intervall, unsigned int timeout);
    void Update();
    unsigned int GetDistance();
    int Measure();

};
