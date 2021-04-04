
#include "Robot.h" 
#include <Arduino.h>
#include <State.h>

Robot::Robot(){

}

void Robot::Setup(State& state){
    this->MainStateMachine.robot = this;

    Serial.println("Setup Complited!");

    this->MainStateMachine.SetStartState(state);
}

void Robot::Loop(){
    AnalogHandler->Update();
    MainStateMachine.Run();
    IMU->Update();
    Movement->Update();
    ObjectDetection->Update();


    if(components[currentComponent]->IsEnabled())
        components[currentComponent]->Update();

    currentComponent++;

    if(currentComponent >= componentCount)
        currentComponent = 0;

}

void Robot::AddComponent(Component& component){
    if(componentCount >= maxComponents){
        Serial.println("!!! TOO MANY COMPONENTS !!!");
        return;
    }

    components[componentCount] = &component;

    Serial.print("Added new Component! - ");
    Serial.print(componentCount + 1);
    Serial.print(" / ");
    Serial.println(maxComponents);
    componentCount++;
}