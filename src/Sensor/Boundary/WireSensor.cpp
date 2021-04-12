#include "WireSensor.h"

    void WireSensor::Setup(byte pin){
        sensor.setPins(pin,A10);
        sensor.speedTest();
        wireOffTimer = millis();
        outsideTimer = millis();
        lastTimePassedBoundar = millis();
    }

    void WireSensor::Update(uint32_t Now){

        if(Now - magUpdateTimer >= 50){
            magUpdateTimer = Now;
            mag = sensor.getMagnitude(0); 
            if(maxMag < abs(mag)){
                maxMag = abs(mag); 
                calibrated = false;
            }

            if(abs(mag) >= abs(((maxMag * 80L) / 100L)) && !wasOverMax){
                wasOverMax = true;
                isNear = !isNear;
            } else {
                if(abs(mag) <= abs(((maxMag * 70L) / 100L))){
                    wasOverMax = false;
                }
            }

            isInside = sensor.isInside(0) && IsActive();

            if(isInside){
                outsideTimer = millis();
            } else {
                isNear = true;
                wasOverMax = false;
            }

            if(lastInside != isInside){
                lastTimePassedBoundar = millis();
            }

            lastInside = isInside;
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
  
        if(isInside){
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
        if(!isInside)
            return millis() - lastTimePassedBoundar;
        
        return 0;
    }

    unsigned long WireSensor::LastTimePassedBoundary(){
        return millis() - lastTimePassedBoundar;
    }