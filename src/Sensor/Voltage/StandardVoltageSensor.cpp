#pragma once
#include "StandardVoltageSensor.h"  
#include "../AnalogSensor/AnalogSensor.h"

void StandardVoltageSensor::Setup(AnalogSensor& analogSensor, float maxVolt, float filterAlpha, unsigned int updateInterval){
    this->analogSensor = &analogSensor;
    this->updateInterval = updateInterval;
    this->factor = (maxVolt * 1000) / 1023;

    this->filter.Setup(filterAlpha, ((this->analogSensor->GetValue() * factor) / 100));
}

void StandardVoltageSensor::Update(){
    Component::Update();
    
    if(millis() - lastUpdate >= updateInterval){
        lastUpdate = millis();
        filter.Filter(((this->analogSensor->GetValue() * factor)));
    }
}

long StandardVoltageSensor::GetMillivoltage(){
    return filter.GetLowPass();
}