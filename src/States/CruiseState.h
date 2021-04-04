#pragma once

#include <State.h>
#include <Arduino.h>
#include "../Robot.h"
#include "../MovementAction.h"


class CruiseState: public State
{
private:
    Robot* robot;

    MovementActionForward actionForward;
    MovementActionStop actionStop;
    MovementActionCurve actionCurve;

    bool headingSetted = false;

    unsigned long timer = 0;

public:
    CruiseState(char* name): State(name){
        
    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionForward.Setup(255, *this->robot->IMU);

        actionStop.SetDuration(100);
        actionStop.SetWaitForMotorStoped(true);

        actionCurve.UsePID(true);
        actionCurve.SetTarget(45);
        actionCurve.SetDegPerSec(4.5);
    }

    void Enter(){
        robot->Movement->CancleAllActions();

        actionForward.UsePID(false);
        
        robot->Movement->AddAction(actionForward);
        headingSetted = false;
        timer = 0;
    }

    void Update(){
        if(timeInState > 1500 && !headingSetted){
             actionForward.SetHeading();
             headingSetted = true;
             actionForward.UsePID(true);
        }   
    }

    void Exit(){
        robot->Movement->CancleAllActions();
    }
};