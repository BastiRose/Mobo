#pragma once

#include <State.h>
#include <Arduino.h>
#include "../Robot.h"
#include "../MovementAction.h"


class AvoidTurnState: public State
{
private:
    Robot* robot;

    MovementActionTurnOnPoint actionTurn;
    MovementActionStop actionStop;
    MovementActionBreak actionBreak;

    int8_t direction = 1;

    int8_t startBoundaryError  = 0;

    bool wasInside = false;
    bool foundTurningDirection = false;

    bool lastInside = false;
public:
    AvoidTurnState(char* name): State(name){
        
    }

    void Setup(Robot& robot){
        this->robot = &robot;

        actionTurn.Setup(255, *this->robot->IMU);
        actionTurn.EnablePID(false, false);
    }

    void Enter(){
        wasInside = false;
        foundTurningDirection = false;
        lastInside = false;

        robot->Movement->CancleAllActions();

        actionTurn.SetTargetAngle(360);
        actionTurn.SetDirection(direction);
       
        actionStop.SetDuration(1);
        actionStop.SetWaitForMotorStoped(true);

        actionBreak.SetDuration(500);

        if(!robot->BoundarySensor->IsInside())
            robot->Movement->AddAction(actionBreak);
        else
            robot->Movement->AddAction(actionStop);

        robot->Movement->AddAction(actionTurn);
    }

    void Update(){

        if(robot->Movement->IsCurrentAction(actionStop) && !wasInside){
            if(!robot->BoundarySensor->IsInside()){
                robot->Movement->CancleAllActions();    
                robot->Movement->AddAction(actionBreak); 
                robot->Movement->AddAction(actionTurn);
            }
        }

        if(robot->Movement->IsCurrentAction(actionBreak)){
            startBoundaryError = robot->BoundarySensor->BoundaryError();
        }

        if(robot->Movement->IsCurrentAction(actionTurn)){

            //If outside of the wire find the fastes direction inside!
            if(!robot->BoundarySensor->IsInside() && !wasInside){
                
                if(!foundTurningDirection && abs(actionTurn.AngleTurned()) > 15 && robot->BoundarySensor->BoundaryError() > startBoundaryError){
                    foundTurningDirection = true;
                    
                    robot->Movement->CancleAllActions();
                    direction = direction * -1;
                    actionTurn.SetDirection(direction);
                    actionTurn.SetTargetAngle(360);
                    robot->Movement->AddAction(actionTurn);
                }
                
            } else {

                //Turn
                if(!wasInside){
                    robot->Movement->CancleAllActions();
                    actionTurn.SetTargetAngle(random(10, 45));
                    robot->Movement->AddAction(actionTurn);
                    //robot->Movement->AddAction(actionStop);
                }

                wasInside = true;

                //Prevent to move outside of the wire again
                if(!robot->BoundarySensor->IsInside() && lastInside && abs(actionTurn.AngleTurned()) > 5){
                    robot->Movement->CancleAllActions();
                    lastInside = false;
                    direction *= -1;
                    actionTurn.SetDirection(direction);
                    actionTurn.SetTargetAngle(random(5,25));
                    robot->Movement->AddAction(actionTurn);
                    //robot->Movement->AddAction(actionStop);
                }

                if(robot->BoundarySensor->IsInside()){
                    lastInside = true;
                }
            }
        }

        if(!robot->Movement->HasActionsLeft()){
            Serial.println("AVOID TRUEN State Done!");
            done = true;
        }
    }

    void Exit(){
        robot->Movement->CancleAllActions();
    }
};