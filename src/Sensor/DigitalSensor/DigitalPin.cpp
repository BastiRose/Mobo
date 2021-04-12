#include "DigitalPin.h"

void DigitalPin::Setup(byte pin, unsigned int timeToActive, bool activeOn){
    this->pin = pin;
    this->reverse = !activeOn;
    this->timeToActive = timeToActive;
    
    pinMode(pin, INPUT);

    
}

void DigitalPin::Update(uint32_t now){

    if(!(reverse? !digitalRead(pin): digitalRead(pin))){
        activeTimer = now;
    }

    active = (now - activeTimer) >= timeToActive;
    
}

bool DigitalPin::IsActive(){
    return active;
}