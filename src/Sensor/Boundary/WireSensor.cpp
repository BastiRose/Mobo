#include "WireSensor.h"

    void WireSensor::Setup(byte pin){
        sensor.setPins(pin,A10);
        sensor.speedTest();
        wireOffTimer = millis();
        outsideTimer = millis();
    }

    void WireSensor::Update(){
        Component::Update();

        if(millis() - magUpdateTimer >= 50){
            magUpdateTimer = millis();
            mag = sensor.getMagnitude(0); 
            if(maxMag < abs(mag)){
                maxMag = abs(mag); 
                calibrated = false;
            }
            
             

            if(abs(mag) >= abs(((maxMag * 90L) / 100L)) && !wasOverMax){
                wasOverMax = true;
                isNear = !isNear;
            } else {
                if(abs(mag) <= abs(((maxMag * 80L) / 100L))){
                    wasOverMax = false;
                }
            }

            if(IsInside()){
                isNear = true;
                wasOverMax = false;
                outsideTimer = millis();
            }
        }
    }

    bool WireSensor::IsActive(){

        if(sensor.getSmoothMagnitude(0) < 100){

            if(millis() -  wireOffTimer >= 2000){
               wireActive = false;
            }

        } else {
            wireOffTimer = millis();
            wireActive = true;
        }

        return wireActive;
    }

    bool WireSensor::IsInside(){
  
        if(sensor.isInside(0) && IsActive()){
            return true;
        }

        calibrated = true;
        return false;
    }

    bool WireSensor::IsNear(){
        if(calibrated)
            return isNear;

        return false;
    }

    int WireSensor::BoundaryError(){
        return mag;
    }

    int WireSensor::BoundaryErrorMax(){
        return maxMag;
    }

    unsigned long WireSensor::TimeOutside(){
        return millis() - outsideTimer;
    }