#pragma once
#include <arduino.h>
#include "Sensor/Voltage/VoltageSensor.h"
#include "Sensor/DigitalSensor/DigitalSensor.h"
#include "Component.h"

class BatterySystem: public Component
{
    private:
        VoltageSensor* sensor;
        DigitalSensor* chargingSensor;

        int full = 0;
        int minToReady = 0;
        int low = 0;
        int critical = 0;

    public:
        void Setup(VoltageSensor& sensor, DigitalSensor& chargingSensor, int full, int minToReady, int low, int critical){
            this->sensor = &sensor;
            this->chargingSensor = &chargingSensor;
            this->full = full;
            this->minToReady = minToReady;
            this->low = low;
            this->critical = critical;
        }

        bool IsFull(){
            return (sensor->GetMillivoltage() >= full);
        }

        bool IsReady(){
            return (sensor->GetMillivoltage() >= minToReady);  
        }

         bool IsLow(){
            return (sensor->GetMillivoltage() <= low);
        }

         bool IsCritical(){
            return (sensor->GetMillivoltage() <= critical);
        }

        byte Capacity(){
            if(sensor->GetMillivoltage() <= critical)
                return 0;

            if(sensor->GetMillivoltage() >= full)
                return 100;

            return (byte)map(sensor->GetMillivoltage(), critical, full, 0, 100);
        }

        float GetVoltage(){
            return ((float)sensor->GetMillivoltage() / 1000.0);
        }

        int GetMillivoltage(){
            return sensor->GetMillivoltage();
        }

        int GetMaxMillivoltage(){
            return full;
        }

        int GetMaxVoltage(){
            return full * 1000;
        }

        bool IsCharging(){
            return chargingSensor->IsActive();
        }

        void Update(uint32_t now ){
            
        }
};


