#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class PrepareMowingState: public State
{
private:
    Robot* robot;

public:
    PrepareMowingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
    }

    void Enter(){
        robot->MowerMotor->SetMowerActive(true);
    }

    void Update(){
        if(robot->MowerMotor->IsMowerReady())
            done = true;
    }

    void Exit(){
      
    }
};