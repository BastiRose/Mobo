#include "RN_VNH2.h"
#include <Arduino.h>
#include "../Sensor/Current/CurrentSensor.h"

void RN_VNH2::Setup(byte pinA, byte pinB, byte pinPWM){
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinPWM = pinPWM;  

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);

    reversed = false;
    
    Brake();
}

void RN_VNH2::Update(){
    Component::Update();
    
    if(speed != tmpSpeed){
        tmpSpeed = speed;

        if(brake){
            setPinsForSpin();
            brake = false;
        }

        if(speed < 0 && reversed == false){
            changeDirection();
            setPinsForSpin();
        } else {
            if(speed > 0 && reversed == true){
                changeDirection();
                setPinsForSpin();
            }
        }

        analogWrite(pinPWM, abs(speed));
    }
}

void RN_VNH2::ChangeSpeed(const int speed){
    this->speed = speed;
}


void RN_VNH2::Brake(){
    speed = 0;
    tmpSpeed = 0;

    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    analogWrite(pinPWM, 0);
    brake = true;
}

int RN_VNH2::GetSpeed(){
    return speed;
}

void RN_VNH2::changeDirection(){
        reversed = !reversed;
        byte tmp = pinA;
        pinA = pinB;
        pinB = tmp;
}

void RN_VNH2::setPinsForSpin(){
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
}

void RN_VNH2::Enable(bool enable){
    if(enable){
        setPinsForSpin();
    } else {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    }
}