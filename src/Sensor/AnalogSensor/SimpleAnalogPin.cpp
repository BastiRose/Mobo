#include "SimpleAnalogPin.h"
#include <ADCHandler.h>

void SimpleAnalogPin::Setup(byte pin, ADCHandler& adcHandler, bool calibrate){
    this->pin = pin;
    this->adcHandler = &adcHandler;

    this->adcHandler->SetCapture(pin, 1, calibrate);
}

void SimpleAnalogPin::Update(uint32_t now){

}

int SimpleAnalogPin::GetValue(){
    return adcHandler->Read(pin);
}