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

        bool needCharging = false;

        long millivolt = 0;

        unsigned long needChargingTimer = 0;

    public:
        void Setup(VoltageSensor& sensor, DigitalSensor& chargingSensor, int full, int minToReady, int low, int critical){
            this->sensor = &sensor;
            this->chargingSensor = &chargingSensor;
            this->full = full;
            this->minToReady = minToReady;
            this->low = low;
            this->critical = critical;
            needChargingTimer = millis();
        }

        bool IsFull(){
            return (millivolt >= full);
        }

        bool NeedCharging(){
            return needCharging;  
        }

         bool IsLow(){
            return (millivolt <= low);
        }

         bool IsCritical(){
            return ( millivolt <= critical);
        }

        byte Capacity(){
            if(millivolt <= critical)
                return 0;

            if( millivolt >= full)
                return 100;

            return (byte)map(millivolt, critical, full, 0, 100);
        }

        float GetVoltage(){
            return ((float)millivolt/ 1000.0);
        }

        int GetMillivoltage(){
            return  millivolt;
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
            millivolt = sensor->GetMillivolt();

            if(IsLow()){
                if(now - needChargingTimer > 10000){
                    needCharging = true;
                } 
            } else {
                needChargingTimer = now;
            }

            if(millivolt >= minToReady){
                needCharging = false;
            } 
        }
};


