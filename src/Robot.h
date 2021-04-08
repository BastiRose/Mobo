#pragma once

#include "Driver/MotorDriver.h"
#include "Sensor/Distance/DistanceSensor.h"
#include "Sensor/DigitalSensor/DigitalSensor.h"
#include "Sensor/Voltage/VoltageSensor.h"
#include <Arduino.h>
#include <StateMachine.h>
#include <ADCHandler.h>
#include "Motor.h"
#include "MovementSystem.h"
#include "Component.h"
#include "BatterySystem.h"
#include "ObjectDetectionSystem.h"
#include "Sensor/IMU/IMU.h"
#include "Sensor/Boundary/BoundarySensor.h"
#include "MowerMotorSystem.h"
#include "TaskManager.h"

class State;

class Robot
{
    private:

    byte componentCount = 0; 
    byte currentComponent = 0; 
    const byte maxComponents = 22; 
    Component* components[22];

    byte currentLowPrioComponent = 0;
    bool lowPrioUpdated = false;
    
    public:

        unsigned long Now = 0;

        unsigned int stateUpdates = 0;

        Robot();
        StateMachine MainStateMachine;

        ADCHandler* AnalogHandler;
        MovementSystem* Movement;
        BatterySystem* Battery;
        ObjectDetectionSystem* ObjectDetection;
        BoundarySensor* BoundarySensor;
        IMU* IMU;
        MowerMotorSystem* MowerMotor;
        TaskManager* Tasks;
        


        void Setup(State& state);
        void Loop();
        void AddComponent(Component& Component);
        int Counter;
};