#pragma once

#include <State.h>
#include <Arduino.h>
#include "../Robot.h"
#include "../MovementAction.h"


class AvoidBackState: public State
{
private:
    Robot* robot;

    MovementActionStop actionStop;
    MovementActionBackwards actionBackwards;
    MovementActionTurnOnPoint actionTurn;

public:
    AvoidBackState(char* name): State(name){
        
    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionBackwards.Setup(255, *this->robot->IMU);

        actionTurn.Setup(255, *this->robot->IMU);
        actionTurn.EnablePID(true, false);

        actionStop.SetDuration(100);
        actionStop.SetWaitForMotorStoped(true);
    }

    void Enter(){
        actionTurn.SetDirection(0);
        actionTurn.SetTargetAngle(random(45, 120));

        robot->Movement->CancleAllActions();
        robot->Movement->AddAction(actionStop);
        robot->Movement->AddAction(actionBackwards);
        robot->Movement->AddAction(actionTurn);
        //robot->Movement->AddAction(actionStop);
    }

    void Update(){
        if(robot->Movement->GetTimeInCurrentAction() > 1000 && robot->Movement->IsCurrentAction(actionBackwards))
            robot->Movement->CancleCurrentAction();

        if(!robot->Movement->HasActionsLeft()){
            Serial.println("AVOID State Done!");
            done = true;
        }
    }

    void Exit(){
        robot->Movement->CancleAllActions();
    }
};