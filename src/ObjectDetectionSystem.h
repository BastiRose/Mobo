#pragma once
#include "Sensor/Distance/DistanceSensor.h"
#include "Sensor/Current/CurrentSensor.h"
#include <arduino.h>

class ObjectDetector{

    public:
        int Angle = 0;
        unsigned int Distance = 0; 
        virtual bool Ckeck() = 0;
};

class RangeObjectDetector : public ObjectDetector{
    
    private:
        DistanceSensor* sensor;
        unsigned int minDistance = 0;

    public:
        void Setup(DistanceSensor& sensor, unsigned int minDistance, int Angle){
            this->sensor = &sensor;
            this->minDistance = minDistance;
            this->Angle = Angle;
        }

        bool Ckeck(){
            this->Distance = sensor->GetDistance();
            return this->Distance <= this->minDistance;
        }
};

class MotorCurrentSensing : public ObjectDetector{
    
    private:
        Motor* motor;
        CurrentSensor* sensor;
        BatterySystem* battery;
        int triggerMilliAmp = 0;

        byte triggerLoad = 0;
        byte triggerLoadSoft = 0;
        unsigned int softTriggertime = 0;
        unsigned long timer = 0;
        bool isTriggered = false;

        int counter = 0;

    public:
        void Setup(BatterySystem& battery, CurrentSensor& sensor, Motor& motor, int triggerLoad){
            this->battery = &battery;
            this->sensor = &sensor;
            this->motor = &motor;
            this->Angle = 0;

            this->triggerMilliAmp = round((this->motor->GetMaxMilliAmp()) * (triggerLoad / 100.0));
        }

        bool Ckeck(){
            if(millis() - timer >= 50 ){
                timer = millis();

                isTriggered = false;
                if(motor->GetSpeed() > 50 && !battery->IsCritical()){
                    if(map(abs(motor->GetSpeed()), 0, motor->GetMaxSpeed(), 10, (((float)triggerMilliAmp) / battery->GetMaxMillivoltage()) * battery->GetMillivoltage()) <= sensor->GetCurrent()){
                        counter++;
                        if(counter >= 20){
                            isTriggered = true;
                        }
                        
                    } else {
                        counter = 0;
                    }
                }
            }

            return isTriggered;
        }
};

class ObjectDetectionSystem {
private:
    
    ObjectDetector* detectors[3];
    ObjectDetector* closest;

    const byte maxDetectors = 3;
    byte detectorCount = 0;
    byte currentDetector = 0;

    bool hasObjectDetected = false;
public:

    void AddDetector(ObjectDetector& detector){
        
        if(detectorCount >= maxDetectors){
            Serial.println("!!! TOO MANY OBJECT DETECTORS !!!");
            return;
        }

        detectors[detectorCount] = &detector;
        closest = detectors[detectorCount];

        Serial.print("Added new Object Detector! - ");
        Serial.print(detectorCount + 1);
        Serial.print(" / ");
        Serial.println(maxDetectors);

        detectorCount++;
    }

    void Update(){

        hasObjectDetected = false;
        
        for(currentDetector = 0; currentDetector < detectorCount; currentDetector++){
            if(detectors[currentDetector]->Ckeck()){
                hasObjectDetected = true;
                if(detectors[currentDetector]->Distance < closest->Distance || !closest->Ckeck()){
                    closest = detectors[currentDetector];
                }
            }
        }


    }

    bool HasObjectDetected(){
        return hasObjectDetected;
    }

    void GetClosestObject(int& outDistance, int& outAngle){
        outDistance = closest->Distance;
        outAngle = closest->Angle;
    }

    ObjectDetector& GetClosestObject(){
        return *closest;
    }
    
};