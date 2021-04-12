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
    float lastAngleError = 0;



public:
    CruiseState(char* name): State(name){
        
    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionForward.Setup(255, *this->robot->IMU);
        actionCurve.Setup(255,  *this->robot->IMU);

        actionStop.SetDuration(100);
        actionStop.SetWaitForMotorStoped(true);
    }

    void Enter(){
        robot->Movement->CancleAllActions();

        actionForward.UsePID(false);
        
        robot->Movement->AddAction(actionForward);
        headingSetted = false;

        actionCurve.SetDir(0);
        actionCurve.SetTarget(45);
        timer = millis();
        
    }

    void Update(){
        if(timeInState > 1500 && !headingSetted){
             actionForward.SetHeading();
             headingSetted = true;
             actionForward.UsePID(true);
             timer = millis();
        }   

        if(abs(actionForward.GetError()) < 150){
            timer = millis();
        }

        if(millis() - timer > 3000){
            done = true;
        }

        /*if(robot->ObjectDetection->HasObjectDetected() && robot->ObjectDetection->GetClosestObject().Distance < 35){
            actionCurve.SetDegPerSec( 40 - ((30 / 35) * robot->ObjectDetection->GetClosestObject().Distance));

            if(!robot->Movement->IsCurrentAction(actionCurve)){
                robot->Movement->CancleAllActions();
                robot->Movement->AddAction(actionCurve);
                robot->Movement->AddAction(actionForward);
            }
        }*/
    }

    void Exit(){
        robot->Movement->CancleAllActions();
    }
};