#pragma once

#include <State.h>

#include "../MovementAction.h"
#include "../Tasks.h"


class BootingState: public State
{
private:
    Robot* robot;


    TaskMow taskMow;
    TaskGoHome taskGoHome;
    TaskMowBoundary taskMowBoundary;
 


    unsigned long timer;
    uint8_t counter = 0;

public:
    BootingState(char* name): State(name){

    }

    void Setup(Robot& robot){
        this->robot = &robot;
        taskMowBoundary.Setup(*this->robot->IMU);
    }

    void Enter(){
        timer = millis();
        Serial.println("Booting...");
        counter = 0;
        
    }

    void Update(){

        if(millis() - timer >= 1000L * counter){
            counter++;
            robot->MowerMotor->Beep(500);        
        }
            

        if(millis() - timer >= 5000){
             counter = 0;
            if(robot->BoundarySensor->IsActive()){
               this->done = true;

                if(!robot->Battery->IsCharging()){
                    robot->Tasks->AddTask(taskMowBoundary);
                    taskMow.Setup(0);
                    robot->Tasks->AddTask(taskMow);
                }
            }



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

            Serial.print("Task: Type: ");
            Serial.print((int)robot->Tasks->GetCurrentTaskType());
            Serial.print(" Done: ");
            Serial.println(!robot->Tasks->IsCurrentTask(TaskMow::Type));
            
        }
    }

    void Exit(){
        Serial.println("Booting Done!");
    }
};