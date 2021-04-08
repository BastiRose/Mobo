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

public:
    DockingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;

        actionBreak.SetDuration(1000);
        actionForward.UsePID(false);
        actionForward.Setup(180, *this->robot->IMU);
        actionBackwards.Setup(180, *this->robot->IMU);
        actionBreak.SetDuration(2000);
    }

    void Enter(){
        robot->MowerMotor->SetMowerActive(false);
        robot->Movement->CancleAllActions();
        robot->Movement->AddAction(actionBreak);

        if(robot->Tasks->IsCurrentTask(TaskMowBoundary::Type))
            robot->Tasks->CancleCurrentTasks();
    }

    void Update(){

        if(!robot->Movement->IsCurrentAction(actionBreak) && robot->Battery->IsCharging()){
            robot->Movement->CancleAllActions();
            robot->Movement->AddAction(actionBreak);
        }

        if(robot->Movement->IsCurrentAction(actionBreak) && !robot->Battery->IsCharging()){
             robot->Movement->AddAction(actionBackwards);
             robot->Movement->AddAction(actionForward);
             robot->Movement->AddAction(actionBreak);
        }


        if(robot->Movement->GetTimeInCurrentAction() >  (robot->Movement->IsCurrentAction(actionForward)? 2000 : 1000)){
            robot->Movement->CancleCurrentAction();
        }

        if(!robot->Movement->HasActionsLeft())
            done = true;

    }

    void Exit(){
      
    }
};