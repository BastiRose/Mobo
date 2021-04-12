#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class ErrorState: public State
{
private:
    Robot* robot;
    MovementActionForward forwad;

public:
    ErrorState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
        forwad.Setup(255, *robot.IMU);
    }

    void Enter(){
        Serial.println("ERROR");
        robot->Movement->CancleAllActions();
        robot->MowerMotor->EmergencyStop();
        //robot->Movement->AddAction(forwad);
    }

    void Update(){


    }

    void Exit(){
      
    }
};