#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"

class Robot;

class ErrorState: public State
{
private:
    Robot* robot;

public:
    ErrorState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
    }

    void Enter(){
        Serial.println("ERROR");
        robot->Movement->CancleAllActions();
        robot->MowerMotor->EmergencyStop();
    }

    void Update(){

    }

    void Exit(){
      
    }
};