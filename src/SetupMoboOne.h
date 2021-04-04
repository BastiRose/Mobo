#pragma once
#include "Robot.h"

#include <Wire.h>

#include <StateTransition.h>
#include <StateMachine.h>
#include "States/BootingState.h"
#include "States/CruiseState.h"
#include "States/AvoidBackState.h"
#include "States/AvoidTurnState.h"
#include "States/ErrorState.h"

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

BootingState MoboBootingState("Booting");
CruiseState MoboCruiseState("Cruise");
AvoidBackState MoboAvoidBackSate("AvoidBack");
AvoidTurnState MoboAvoidTurnSate("AvoidTurn");
ErrorState MoboErrorState("Error");

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

WireSensor WireSensor;

MowerMotorSystem MowerMotorSystem;

MovementSystem Movement;
MovementActionBreak Idle;

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

    //Motor Mower
    MotorDriverMower.Setup(28,29,11,12);
    MotorMowerCurrentPin.Setup(A2, AnalogHandler, true);
    CurrentSensorMotorMower.Setup(MotorMowerCurrentPin, 0.2, 0.80, 200);
    MotorMower.Setup(MotorDriverMower, 255, 30000, 1500);

    robot.AddComponent(MotorDriverMower);
    robot.AddComponent(MotorMower);

    MowerMotorSystem.Setup(Battery, CurrentSensorMotorMower, MotorMower);

    //Sensors
    UsSensor.Setup(51, 53, 250, 10000);

    robot.AddComponent(UsSensor);

    //Analog
    BattPin.Setup(A8, AnalogHandler, true);
    LipoVoltageSensor.Setup(BattPin, 13.7f, 0.40, 250);

    robot.AddComponent(BattPin);
    robot.AddComponent(LipoVoltageSensor);

    Battery.Setup(LipoVoltageSensor, 12600, 12000, 11000, 10500);

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

    IMU.Setup();

    WireSensor.Setup(A9);
    robot.AddComponent(WireSensor);

    //Assign Components to Robot
    robot.AnalogHandler = &AnalogHandler;
    robot.Movement = &Movement;
    robot.Battery = &Battery;
    robot.ObjectDetection = &ObjectDetection;
    robot.IMU = &IMU;
    robot.BoundarySensor = &WireSensor;
    robot.MowerMotor = &MowerMotorSystem;

    setupStates(robot);

    robot.Setup(MoboBootingState);
}

void setupStates(Robot& robot){

    auto ConditionStateDone = [] (Robot& robot) -> bool{
        return robot.MainStateMachine.currentState->done;
    };

    auto ConditionToError = [] (Robot& robot) -> bool{
        return  robot.BoundarySensor->TimeOutside() > 10000 || 
                abs(robot.IMU->GetRoll()) > 45 || 
                abs(robot.IMU->GetPitch()) > 45 || 
                robot.MainStateMachine.currentState->timeInState > 120000L;
    };

    /*Booting*/
    StateTransition BootingTransitions[] = {{&MoboCruiseState, ConditionStateDone}};
    MoboBootingState.SetTransitions(BootingTransitions, 1); 
    MoboBootingState.Setup(robot);

    /*Cruise*/
    auto ConditionCruiseToAvoidBackCollision = [] (Robot& robot) -> bool{
        return robot.ObjectDetection->HasObjectDetected() && robot.ObjectDetection->GetClosestObject().Distance < 10;
    };

    auto ConditionCruiseToAvoidTurn = [] (Robot& robot) -> bool{
       return (!robot.BoundarySensor->IsInside()) ||
       (robot.ObjectDetection->HasObjectDetected() && (robot.ObjectDetection->GetClosestObject().Distance > 10 && robot.ObjectDetection->GetClosestObject().Distance < 30));
    };

    StateTransition CruiseTransitions[] = {
        {&MoboAvoidTurnSate, ConditionCruiseToAvoidTurn},
        {&MoboAvoidBackSate, ConditionCruiseToAvoidBackCollision},
        {&MoboErrorState, ConditionToError}   
    };

    MoboCruiseState.SetTransitions(CruiseTransitions, 3); 
    MoboCruiseState.Setup(robot);

    /*AvoidBack*/
    StateTransition AvoidBackTransitions[] = {
        {&MoboCruiseState, ConditionStateDone},
        {&MoboErrorState, ConditionToError}  
    };
    MoboAvoidBackSate.SetTransitions(AvoidBackTransitions, 2); 
    MoboAvoidBackSate.Setup(robot);

     /*AvoidTurn*/
    auto ConditionAvoidTurnToAvoidBack = [] (Robot& robot) -> bool{
        return (robot.ObjectDetection->HasObjectDetected() && robot.ObjectDetection->GetClosestObject().Distance < 5) ||
            (!robot.BoundarySensor->IsInside() && robot.MainStateMachine.currentState->done);
    };

    StateTransition AvoidTurnTransitions[] = {
        {&MoboAvoidBackSate, ConditionAvoidTurnToAvoidBack}, 
        {&MoboCruiseState, ConditionStateDone},
        {&MoboErrorState, ConditionToError}  
    };

    MoboAvoidTurnSate.SetTransitions(AvoidTurnTransitions, 3); 
    MoboAvoidTurnSate.Setup(robot);

    /*Error*/
    MoboErrorState.Setup(robot);
}