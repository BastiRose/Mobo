#include "DigitalPin.h"

void DigitalPin::Setup(byte pin, bool activeOn){
    this->pin = pin;
    this->reverse = !activeOn;
    
    pinMode(pin, INPUT);
}

void DigitalPin::Update(uint32_t now){

}

bool DigitalPin::IsActive(){
    return reverse? !digitalRead(pin): digitalRead(pin);
}