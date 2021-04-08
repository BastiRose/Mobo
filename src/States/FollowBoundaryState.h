#pragma once

#include <State.h>
#include "../Robot.h"
#include "../MovementAction.h"


class FollowBoundaryState: public State
{
private:
    Robot* robot;
    MovementActionFollowBoundary actionFollowBoundary;
    MovementActionBreak actionBreak;

public:
    FollowBoundaryState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionFollowBoundary.Setup(255, *this->robot->BoundarySensor);
        actionBreak.SetDuration(500);
        
    }

    void Enter(){
        robot->Movement->CancleAllActions();
        robot->Movement->AddAction(actionBreak);
        robot->Movement->AddAction(actionFollowBoundary);

        if(this->robot->Tasks->IsCurrentTask(TaskMowBoundary::Type)){
            this->robot->Tasks->StartCurrentTask();
        }
    }

    void Update(){

    }

    void Exit(){
        robot->Movement->CancleAllActions();
    }
};