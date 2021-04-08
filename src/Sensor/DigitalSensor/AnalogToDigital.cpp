#include "AnalogToDigital.h"
#include "../AnalogSensor/AnalogSensor.h"

void AnalogToDigital::Setup(AnalogSensor& analogSensor, int threshold, bool activeHigher){
    this->analogSensor = &analogSensor;
    this->threshold = threshold;
    this->activeHigher = activeHigher;
}

void AnalogToDigital::Update(uint32_t now){

}

bool AnalogToDigital::IsActive(){
    return activeHigher? (analogSensor->GetValue() >= threshold) : (analogSensor->GetValue() <= threshold);

}