#include "AnalogSensor.h"
#include <arduino.h>

class ADCHandler;

class SimpleAnalogPin : public AnalogSensor
{

    private:
        byte pin;
        ADCHandler* adcHandler;
    public:
        void Setup(byte pin, ADCHandler& adcHandler, bool calibrate);
        void Update();
        int GetValue();
};