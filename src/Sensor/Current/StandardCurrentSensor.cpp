#include "StandardCurrentSensor.h"
#include "../AnalogSensor/AnalogSensor.h"

void StandardCurrentSensor::Setup(AnalogSensor& analogSensor, float factor, float filterAlpha, unsigned int updateInterval){
    this->analogSensor = &analogSensor;
    this->updateInterval = updateInterval;
    this->factor = (long)(factor * 5.0 * 1000.0) ;
    this->filter.Setup(filterAlpha, (this->analogSensor->GetValue() * factor));
}

void StandardCurrentSensor::Update(uint32_t now){

    if(now - lastUpdate >= updateInterval){
        lastUpdate = millis();
        filter.Filter((analogSensor->GetValue() * factor));
        currentValue = filter.GetLowPass() / 1000;
    }
}

unsigned int StandardCurrentSensor::GetCurrent(){
    return currentValue;
}

unsigned int StandardCurrentSensor::GetPower(int milliVolt){
    return milliVolt * currentValue;
}