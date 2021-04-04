#include "HC_SR04.h"


void HC_SR04::Setup(byte echoPin, byte triggerPin, unsigned int intervall, unsigned int timeout){
    this->echo = echoPin;
    this->trigger = triggerPin;
    this->intervall = intervall;
    this->timeout = timeout;
    this->measuredDistance = UINT_MAX;

    pinMode(trigger, OUTPUT);  
    pinMode(echo , INPUT);
}

void HC_SR04::Update(){
    Component::Update();
    if(millis() - lastMeasurement > intervall){
        Measure();
    }
}

unsigned int HC_SR04::GetDistance(){
    return measuredDistance;
}

int HC_SR04::Measure(){
    lastMeasurement = millis();

    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    unsigned long duration = pulseIn(echo, HIGH, timeout);
    measuredDistance = duration * 0.0346 / 2;

    if(measuredDistance == 0)
        measuredDistance = UINT_MAX;

    return measuredDistance;
}