#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class PrepareMowingState: public State
{
private:
    Robot* robot;
    uint8_t counter = 0;

public:
    PrepareMowingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
    }

    void Enter(){

        robot->MowerMotor->ChangeDirection();
        counter = 0;
    }

    void Update(){

        if(timeInState >= 1000L * counter && counter != 5){
            counter++;
            robot->MowerMotor->Beep(500);        
        }

        if(counter == 5){
            if(!robot->MowerMotor->IsMowerActive())
                robot->MowerMotor->SetMowerActive(true);

            if(robot->MowerMotor->IsMowerReady())
                done = true;
        }




    }

    void Exit(){
      
    }
};