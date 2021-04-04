#pragma once

#include <State.h>

#include "../MovementAction.h"

class Robot;

class BootingState: public State
{
private:
    Robot* robot;
    MovementActionTurnOnPoint actionTurn;
    MovementActionBackwards actionBackwards;
    MovementActionForward actionForward;

    int dir = 1;

    unsigned long timer;

public:
    BootingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
        actionTurn.Setup(255, *this->robot->IMU);
        actionBackwards.Setup(255, *this->robot->IMU);
        actionForward.Setup(255, *this->robot->IMU);

        actionTurn.EnablePID(true, true);
        actionTurn.SetDirection(dir);
        actionTurn.SetTargetAngle(90);

        dir = this->robot->BoundarySensor->BoundaryError();
        actionForward.UsePID(true);
        //this->robot->Movement->AddAction(actionTurn);

 
    }

    void Enter(){
        timer = millis();
        Serial.println("Booting...");
    }

    void Update(){
            
            if(robot->ObjectDetection->HasObjectDetected())
                Serial.println(robot->ObjectDetection->GetClosestObject().Distance);

        if(millis() - timer >= 5000){

           if(robot->BoundarySensor->IsActive())
               this->done = true;



            timer = millis();
            Serial.print("Battery Capacity: [ ");
            Serial.print(robot->Battery->Capacity());
            Serial.print("% ] Voltage: ");
            Serial.println(robot->Battery->GetVoltage());

            Serial.print("IMU: Roll: ");
            Serial.print(robot->IMU->GetRoll());
            Serial.print(" PITCH: ");
            Serial.print(robot->IMU->GetPitch());
            Serial.print(" YAW: ");
            Serial.println(robot->IMU->GetYaw());

            Serial.print("Wire: Active: ");
            Serial.print(robot->BoundarySensor->IsActive());
            Serial.print(" Inside: ");
            Serial.println(robot->BoundarySensor->IsInside());
            
        }
    }

    void Exit(){
        Serial.println("Booting Done!");
    }
};