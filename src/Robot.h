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

class State;

class Robot
{
    private:

    byte componentCount = 0; 
    byte currentComponent = 0; 
    const byte maxComponents = 16; 
    Component* components[16];

    public:
        Robot();
        StateMachine MainStateMachine;

        ADCHandler* AnalogHandler;
        MovementSystem* Movement;
        BatterySystem* Battery;
        ObjectDetectionSystem* ObjectDetection;
        BoundarySensor* BoundarySensor;
        IMU* IMU;
        MowerMotorSystem* MowerMotor;
        


        void Setup(State& state);
        void Loop();
        void AddComponent(Component& Component);
        int Counter;
};