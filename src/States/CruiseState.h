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

    bool headingSetted = false;

    unsigned long timer = 0;

    unsigned long maxTimeToBoundary = 30000;

    int counter = 0;


public:
    CruiseState(char* name): State(name){
        
    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionForward.Setup(255, *this->robot->IMU);
    }

    void Enter(){
        robot->Movement->CancleAllActions();

        actionForward.UsePID(false);
        
        robot->Movement->AddAction(actionForward);
        headingSetted = false;

        timer = millis();
        
    }

    void Update(){
        if(timeInState > 1500 && !headingSetted){
             actionForward.SetHeading();
             headingSetted = true;
             actionForward.UsePID(true);
             timer = millis();
        }   

        if(abs(actionForward.GetError()) < 125){
            timer = millis();
        }

        if(millis() - timer > 3000){
            done = true;
        }

        if(timeInState > maxTimeToBoundary * 1.3f){
            done = true;
            counter++;
        }
    }

    void Exit(){
        if(!robot->BoundarySensor->IsInside()){
            counter = 0;
            if(maxTimeToBoundary < timeInState)
                maxTimeToBoundary = timeInState;
        }

        if(counter >= 3)
            failed = true;

        robot->Movement->CancleAllActions();
    }
};