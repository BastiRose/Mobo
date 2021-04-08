#pragma once
#define sizeofArray(x) (sizeof(x) / sizeof(x[0]))
#include "Robot.h"

#include <Wire.h>

#include <StateTransition.h>
#include <StateMachine.h>

#include "States/BootingState.h"
#include "States/CruiseState.h"
#include "States/AvoidBackState.h"
#include "States/AvoidTurnState.h"
#include "States/ErrorState.h"
#include "States/DockingState.h"
#include "States/ExitDockState.h"
#include "States/SleepState.h"
#include "States/FollowBoundaryState.h"
#include "States/PrepareMowingState.h"


#include "Driver/MotorDriver.h"
#include "Driver/RN_VNH2.h"
#include "Driver/bts7960.h"
#include "Motor.h"

#include "Sensor/Distance/HC_SR04.h"
#include "Sensor/DigitalSensor/DigitalPin.h"
#include "Sensor/DigitalSensor/AnalogToDigital.h"

#include <ADCHandler.h>
#include <ADCManWrapper.h>
#include <adcman.h>

#include "Sensor/AnalogSensor/SimpleAnalogPin.h"

#include "Sensor/Current/StandardCurrentSensor.h"

#include "Sensor/Voltage/StandardVoltageSensor.h"
#include "MovementSystem.h"
#include "BatterySystem.h"

#include "Sensor/IMU/IMU.h"
#include "Sensor/IMU/GY80.h"

#include "Sensor/Boundary/WireSensor.h"

#include "ObjectDetectionSystem.h"

#include "MovementAction.h"

#include "MowerMotorSystem.h"

#include "TaskManager.h"

BootingState MoboBootingState("Booting");
CruiseState MoboCruiseState("Cruise");
AvoidBackState MoboAvoidBackSate("AvoidBack");
AvoidTurnState MoboAvoidTurnSate("AvoidTurn");
FollowBoundaryState MoboFollowBoundaryState("FollowBoundary");
DockingState MoboDockingSate("Docking");
ExitDockState MoboExitDockState("ExitDock");
SleepState MoboSleepState("Sleeping");
ErrorState MoboErrorState("Error");
PrepareMowingState MoboPrepareMowingState("PrepareMowing");

RN_VNH2 MotorDriverLeft;
SimpleAnalogPin MotorLeftCurrentPin; 
StandardCurrentSensor CurrentSensorMotorLeft;
Motor MotorLeft;

RN_VNH2 MotorDriverRight;
SimpleAnalogPin MotorRightCurrentPin; 
StandardCurrentSensor CurrentSensorMotorRight;
Motor MotorRight;


bts7960 MotorDriverMower;
SimpleAnalogPin MotorMowerCurrentPin; 
StandardCurrentSensor CurrentSensorMotorMower;
Motor MotorMower;

AnalogToDigital EmergencyButton;
DigitalPin ChargingContact;

HC_SR04 UsSensor;

ADCManWrapper AnalogHandler;

SimpleAnalogPin BattPin; 
StandardVoltageSensor LipoVoltageSensor;
BatterySystem Battery;

ObjectDetectionSystem ObjectDetection;
RangeObjectDetector USDetector;
MotorCurrentSensing MotorLeftSensing;
MotorCurrentSensing MotorRightSensing;

GY80 IMU;

WireSensor WireBoundarySensor;

MowerMotorSystem MowerSystem;

MovementSystem Movement;
MovementActionBreak Idle;

TaskManager Tasks;

void setupStates(Robot& robot);


void SetupRobot(Robot& robot){
    
    AnalogHandler.Setup();


    Wire.setClock(400000);
    Wire.begin();

    //Motor Left
    MotorLeftCurrentPin.Setup(A1, AnalogHandler, true);
    CurrentSensorMotorLeft.Setup(MotorLeftCurrentPin,7.58f, 0.80, 250);
    MotorDriverLeft.Setup(22,23,9);
    MotorLeft.Setup(MotorDriverLeft, 255, 1000, 1200);

    robot.AddComponent(MotorLeftCurrentPin);
    robot.AddComponent(CurrentSensorMotorLeft);
    robot.AddComponent(MotorDriverLeft);
    robot.AddComponent(MotorLeft);

    //Motor Right
    MotorRightCurrentPin.Setup(A0, AnalogHandler, true);
    CurrentSensorMotorRight.Setup(MotorRightCurrentPin, 7.58f, 0.80, 250);
    MotorDriverRight.Setup(24,25,10);
    MotorRight.Setup(MotorDriverRight, 255, 1000, 1200);

    robot.AddComponent(MotorRightCurrentPin);
    robot.AddComponent(CurrentSensorMotorRight);
    robot.AddComponent(MotorDriverRight);
    robot.AddComponent(MotorRight);

    Movement.Setup(Idle, MotorLeft, MotorRight);
    robot.AddComponent(Movement);

    //Motor Mower
    MotorDriverMower.Setup(28,29,11,12);
    MotorMowerCurrentPin.Setup(A2, AnalogHandler, true);
    CurrentSensorMotorMower.Setup(MotorMowerCurrentPin, 0.2, 0.80, 200);
    MotorMower.Setup(MotorDriverMower, 255, 10000, 1500);

    robot.AddComponent(MotorDriverMower);
    robot.AddComponent(MotorMower);

    MowerSystem.Setup(Battery, CurrentSensorMotorMower, MotorMower);
    robot.AddComponent(MowerSystem);

    //Sensors
    UsSensor.Setup(51, 53, 250, 10000);

    robot.AddComponent(UsSensor);

    //Analog
    BattPin.Setup(A8, AnalogHandler, true);
    LipoVoltageSensor.Setup(BattPin, 13.7f, 0.40, 250);

    robot.AddComponent(BattPin);
    robot.AddComponent(LipoVoltageSensor);

    Battery.Setup(LipoVoltageSensor, ChargingContact, 12600, 11500, 11000, 10500);
    robot.AddComponent(Battery);

    //Digital
    ChargingContact.Setup(27, HIGH);
    EmergencyButton.Setup(BattPin, 500, false);

    robot.AddComponent(ChargingContact);
    robot.AddComponent(EmergencyButton);
    
    //Object Detection
    USDetector.Setup(UsSensor, 30, 0);
    MotorLeftSensing.Setup(Battery, CurrentSensorMotorLeft, MotorLeft, 80);
    MotorRightSensing.Setup(Battery, CurrentSensorMotorRight, MotorRight, 80);

    ObjectDetection.AddDetector(USDetector);
    ObjectDetection.AddDetector(MotorLeftSensing);
    ObjectDetection.AddDetector(MotorRightSensing);

    robot.AddComponent(ObjectDetection);

    //IMU
    IMU.Setup();
    IMU.SetPriority(1);
    robot.AddComponent(IMU);
    

    WireBoundarySensor.Setup(A9);
    robot.AddComponent(WireBoundarySensor);

    robot.AddComponent(Tasks);

    //Assign Components to Robot
    robot.AnalogHandler = &AnalogHandler;
    robot.Movement = &Movement;
    robot.Battery = &Battery;
    robot.ObjectDetection = &ObjectDetection;
    robot.IMU = &IMU;
    robot.BoundarySensor = &WireBoundarySensor;
    robot.MowerMotor = &MowerSystem;
    robot.Tasks = &Tasks;

    setupStates(robot);

    robot.Setup(MoboBootingState);
}

void setupStates(Robot& robot){

    auto ConditionStateDone = [] (Robot& robot) -> bool{
        return robot.MainStateMachine.currentState->done;
    };

    auto ConditionToError = [] (Robot& robot) -> bool{
        return  (
                   robot.Battery->IsCritical() ||
                    abs(robot.IMU->GetRoll()) > 45 || 
                    abs(robot.IMU->GetPitch()) > 45 || 
                    robot.MainStateMachine.currentState->timeInState > 120000L ||
                    !robot.BoundarySensor->IsActive() ||
                    robot.Tasks->IsCurrentTask(TaskStop::Type)
                );
    };

    auto ConditionInDock = [] (Robot& robot) -> bool{
        return robot.Battery->IsCharging();
    };

    /*
    #################################################
    #--------------------Booting--------------------#
    #################################################
    */
    StateTransition BootingTransitions[] = {
        {&MoboSleepState, ConditionStateDone}
    };

    MoboBootingState.SetTransitions(BootingTransitions, sizeofArray(BootingTransitions)); 
    MoboBootingState.Setup(robot);

    /*
    #################################################
    #---------------------Cruise--------------------#
    #################################################
    */
    auto ConditionCruiseToAvoidBackCollision = [] (Robot& robot) -> bool{
        return robot.ObjectDetection->HasObjectDetected() && robot.ObjectDetection->GetClosestObject().Distance < 5;
    };

    auto ConditionCruiseToAvoidTurn = [] (Robot& robot) -> bool{
       return (!robot.BoundarySensor->IsInside()) ||
        (robot.ObjectDetection->HasObjectDetected() && (robot.ObjectDetection->GetClosestObject().Distance >= 5 && 
        robot.ObjectDetection->GetClosestObject().Distance < 30));
    };

    auto ConditionCruiseToFollowBoundary = [] (Robot& robot) -> bool{
        return (
            robot.Battery->IsLow() || 
            robot.Tasks->IsCurrentTask(TaskMowBoundary::Type) || 
            robot.Tasks->IsCurrentTask(TaskGoHome::Type)
        ) && 
        !robot.BoundarySensor->IsInside() && 
        robot.MainStateMachine.currentState->timeInState > 500;
    };

    auto ConditionCruiseToSleep = [] (Robot& robot) -> bool{
        return robot.Tasks->IsCurrentTask(TaskPause::Type) && robot.BoundarySensor->IsInside();
    };


    StateTransition CruiseTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboDockingSate, ConditionInDock},
        {&MoboFollowBoundaryState, ConditionCruiseToFollowBoundary},
        {&MoboAvoidTurnSate, ConditionCruiseToAvoidTurn},
        {&MoboAvoidBackSate, ConditionCruiseToAvoidBackCollision},
        {&MoboSleepState, ConditionCruiseToSleep}
    };

    MoboCruiseState.SetTransitions(CruiseTransitions, sizeofArray(CruiseTransitions)); 
    MoboCruiseState.Setup(robot);

    /*
    #################################################
    #-------------------AvoidBack-------------------#
    #################################################
    */

    StateTransition AvoidBackTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboDockingSate, ConditionInDock},
        {&MoboCruiseState, ConditionStateDone}
    };

    MoboAvoidBackSate.SetTransitions(AvoidBackTransitions, sizeofArray(AvoidBackTransitions)); 
    MoboAvoidBackSate.Setup(robot);

    /*
    #################################################
    #-------------------AvoidTurn-------------------#
    #################################################
    */
    auto ConditionAvoidTurnToAvoidBack = [] (Robot& robot) -> bool{
        return (robot.ObjectDetection->HasObjectDetected() && robot.ObjectDetection->GetClosestObject().Distance < 5) ||
            (!robot.BoundarySensor->IsInside() && robot.MainStateMachine.currentState->done);
    };

    StateTransition AvoidTurnTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboDockingSate, ConditionInDock},
        {&MoboAvoidBackSate, ConditionAvoidTurnToAvoidBack}, 
        {&MoboCruiseState, ConditionStateDone}  
    };

    MoboAvoidTurnSate.SetTransitions(AvoidTurnTransitions, sizeofArray(AvoidTurnTransitions)); 
    MoboAvoidTurnSate.Setup(robot);

    /*
    #################################################
    #-----------------FollowBoundary----------------#
    #################################################
    */
    auto ConditionFollowBoundaryToAvoidBack = [] (Robot& robot) -> bool{
        return  (
                    robot.ObjectDetection->HasObjectDetected() && robot.ObjectDetection->GetClosestObject().Distance < 5 ||
                    robot.BoundarySensor->LastTimePassedBoundary() > 5000
                );
    };

    auto ConditionFollowBoundaryToCruse = [] (Robot& robot) -> bool{
        return  (
                    !robot.Tasks->IsCurrentTask(TaskMowBoundary::Type) &&
                    !robot.Tasks->IsCurrentTask(TaskGoHome::Type)
                );
    };

    StateTransition FollowBoundaryTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboDockingSate, ConditionInDock},
        {&MoboAvoidBackSate, ConditionFollowBoundaryToAvoidBack},
        {&MoboCruiseState, ConditionFollowBoundaryToCruse}
    };

    MoboFollowBoundaryState.SetTransitions(FollowBoundaryTransitions, sizeofArray(FollowBoundaryTransitions));
    MoboFollowBoundaryState.Setup(robot);

    /*
    #################################################
    #---------------------Error---------------------#
    #################################################
    */
    MoboErrorState.Setup(robot);

    /*
    #################################################
    #--------------------Docking--------------------#
    #################################################
    */
    StateTransition DockingTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboSleepState, ConditionStateDone}
    };

    MoboDockingSate.SetTransitions(DockingTransitions, sizeofArray(DockingTransitions));
    MoboDockingSate.Setup(robot);

    /*
    #################################################
    #--------------------ExitDock-------------------#
    #################################################
    */
    StateTransition ExitDockTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboPrepareMowingState, ConditionStateDone}
    };

    MoboExitDockState.SetTransitions(ExitDockTransitions, sizeofArray(ExitDockTransitions));
    MoboExitDockState.Setup(robot);

    /*
    #################################################
    #---------------------Sleep---------------------#
    #################################################
    */
    auto ConditionTaskAndInDock = [] (Robot& robot) -> bool{
        return  (robot.Tasks->IsCurrentTask(TaskMow::Type) || 
                robot.Tasks->IsCurrentTask(TaskMowBoundary::Type)) && 
                robot.Battery->IsCharging();
    };

    auto ConditionTaskAndNotInDock = [] (Robot& robot) -> bool{
        return  (robot.Tasks->IsCurrentTask(TaskMow::Type) || 
                robot.Tasks->IsCurrentTask(TaskMowBoundary::Type) ||
                robot.Tasks->IsCurrentTask(TaskGoHome::Type)) && 
                !robot.Battery->IsCharging();
    };

    StateTransition SleepTransitions[] = {
        {&MoboExitDockState, ConditionTaskAndInDock},
        {&MoboPrepareMowingState, ConditionTaskAndNotInDock}
    };

    MoboSleepState.SetTransitions(SleepTransitions, sizeofArray(SleepTransitions));
    MoboSleepState.Setup(robot);

    /*
    #################################################
    #-----------------PrepareMowing-----------------#
    #################################################
    */
    StateTransition PrepareMowingTransitions[] = {
        {&MoboErrorState, ConditionToError},
        {&MoboCruiseState, ConditionStateDone}
    };

    MoboPrepareMowingState.SetTransitions(PrepareMowingTransitions, sizeofArray(PrepareMowingTransitions));
    MoboPrepareMowingState.Setup(robot);

}