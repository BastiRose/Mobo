#include "VoltageSensor.h" 
#include <HighLowPass.h>

class AnalogSensor;

class StandardVoltageSensor : public VoltageSensor
{
    private:
        HighLowPass filter;
        AnalogSensor* analogSensor;
        unsigned long lastUpdate = 0;
        unsigned int updateInterval = 0;
        unsigned int factor = 0;
    public:
        void Setup(AnalogSensor& analogSenso, float maxVolt, float filterAlpha, unsigned int updateInterval);
        void Update(uint32_t now);
        long GetMillivolt();
};



