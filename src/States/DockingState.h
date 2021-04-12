#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class DockingState: public State
{
private:
    Robot* robot;
    MovementActionBreak actionBreak;
    MovementActionForward actionForward;
    MovementActionBackwards actionBackwards;

    uint8_t tries = 0;

public:
    DockingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;

        actionBreak.SetDuration(1000);
        actionForward.UsePID(true);
        actionForward.Setup(180, *this->robot->IMU);
        actionBackwards.Setup(180, *this->robot->IMU);
        actionBreak.SetDuration(2000);
    }

    void Enter(){
        robot->MowerMotor->SetMowerActive(false);
        robot->Movement->CancleAllActions();
        robot->Movement->AddAction(actionBreak);
        actionForward.SetHeading();
        tries = 0;

        if(robot->Tasks->IsCurrentTask(TaskMowBoundary::Type))
            robot->Tasks->CancleCurrentTasks();
    }

    void Update(){

        if(!robot->Movement->IsCurrentAction(actionBreak) && robot->Battery->IsCharging()){
            robot->Movement->CancleAllActions();
            robot->Movement->AddAction(actionBreak);
        }

        if(robot->Movement->IsCurrentAction(actionBreak) && robot->Movement->GetTimeInCurrentAction() > 1000 && !robot->Battery->IsCharging()){
             robot->Movement->AddAction(actionBackwards);
             robot->Movement->AddAction(actionForward);
             robot->Movement->AddAction(actionBreak);
             tries++;
        }


        if(robot->Movement->GetTimeInCurrentAction() >  (robot->Movement->IsCurrentAction(actionForward)? 3000 : 1000)){
            robot->Movement->CancleCurrentAction();
        }

        if(!robot->Movement->HasActionsLeft() || tries > 4)
            done = true;

    }

    void Exit(){
      
    }
};