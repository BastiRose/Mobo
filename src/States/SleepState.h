#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class SleepState: public State
{
private:
    Robot* robot;

public:
    SleepState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
    }

    void Enter(){
        robot->MowerMotor->SetMowerActive(false);
        robot->Movement->CancleAllActions();
    }

    void Update(){

    }

    void Exit(){
      
    }
};