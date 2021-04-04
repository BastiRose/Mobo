#include "bts7960.h"

void bts7960::Setup(byte pinA, byte pinB, byte pinPWMA, byte pinPWMB){
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinPWMA = pinPWMA;
    this->pinPWMB = pinPWMB;  
    
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);

    reversed = false;

    Enable(true);

    Brake();

}

void bts7960::Update(){
    Component::Update();

    if(speed != tmpSpeed){
        tmpSpeed = speed;

        if(brake){
            Enable(true);
            brake = false;
        }

        if(speed < 0 && reversed == false){
            changeDirection();
        } else {
            if(speed > 0 && reversed == true){
                changeDirection();
            }
        }

        analogWrite(pinPWMB, 0);
        analogWrite(pinPWMA, abs(speed));
    }
}

void bts7960::ChangeSpeed(const int speed){
    this->speed = speed;
}

void bts7960::Brake(){
    speed = 0;
    tmpSpeed = 0;
    analogWrite(pinPWMB, 0);
    analogWrite(pinPWMA, 0);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    brake = true;
}

int bts7960::GetSpeed(){
    return speed;
}

void bts7960::changeDirection(){
        reversed = !reversed;
        byte tmp = pinPWMA;
        pinPWMA = pinPWMB;
        pinPWMB = tmp;
}

void bts7960::Enable(bool enable){
    if(enable){
        digitalWrite(pinA, HIGH);
        digitalWrite(pinB, HIGH);
    } else {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    }
}