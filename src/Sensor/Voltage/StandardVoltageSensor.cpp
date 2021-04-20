#pragma once
#include "StandardVoltageSensor.h"  
#include "../AnalogSensor/AnalogSensor.h"

void StandardVoltageSensor::Setup(AnalogSensor& analogSensor, float maxVolt, float filterAlpha, unsigned int updateInterval){
    this->analogSensor = &analogSensor;
    this->updateInterval = updateInterval;
    this->factor = (maxVolt * 1000) / 1024;

    this->filter.Setup(filterAlpha, ((this->analogSensor->GetValue() * factor)));
}

void StandardVoltageSensor::Update(uint32_t now){
    
    if(now - lastUpdate >= updateInterval){
        lastUpdate = millis();
        filter.Filter(((this->analogSensor->GetValue() * factor)));
    }
}

long StandardVoltageSensor::GetMillivolt(){
    return filter.GetLowPass();
}