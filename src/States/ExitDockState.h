#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class ExitDockState: public State
{
private:
    Robot* robot;
    MovementActionStop actionStop;
    MovementActionTurnOnPoint actionTurn;
    MovementActionBackwards actionBackwards;

public:
    ExitDockState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;

      
        actionTurn.EnablePID(true, false);

        actionTurn.Setup(255, *this->robot->IMU);
        actionBackwards.Setup(180, *this->robot->IMU);
        actionStop.SetDuration(500);
        actionStop.SetWaitForMotorStoped(true);
        actionTurn.SetDirection(1);

        if(this->robot->Tasks->IsCurrentTask(TaskMowBoundary::Type))
            actionTurn.SetTargetAngle(random(45, 125));
        else 
            actionTurn.SetTargetAngle(45);

        
    }

    void Enter(){
        robot->MowerMotor->SetMowerActive(false);
        robot->Movement->CancleAllActions();
        robot->Movement->AddAction(actionBackwards);
        robot->Movement->AddAction(actionStop);
        robot->Movement->AddAction(actionTurn);
    }

    void Update(){

        if(robot->Movement->IsCurrentAction(actionBackwards) && robot->Movement->GetTimeInCurrentAction() > 8000)
            robot->Movement->CancleCurrentAction();

        if(!robot->Movement->HasActionsLeft())
            done = true;
    }

    void Exit(){
      robot->Movement->CancleAllActions();
    }
};