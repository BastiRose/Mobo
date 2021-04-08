#pragma once
#include "Sensor/IMU/IMU.h"

typedef enum
{  
   Task_None = 0, 
   Task_Sleep = 1, 
   Task_Mow = 2,
   Task_Go_Home = 3,
   Task_Mow_Boundary = 4,
   Task_Stop = 5

} task_type_t;

class Task
{
private:
   
public:
    virtual void Activate() = 0;
    virtual void Start() = 0;
    virtual bool Check() = 0;
    virtual task_type_t GetType() = 0;
};

class TaskMow : public Task
{
    private:
        unsigned long startTime = 0;
        unsigned long duration = 0;
    public:
        static const task_type_t Type = Task_Mow;

        void Setup(unsigned long duration){
            this->duration = duration; 
        }

        void Start(){

        }

        void Activate(){
            startTime = millis();
        }

        bool Check(){
            if(duration == 0)
                return false;

            return millis() - startTime >= duration;
        }

        task_type_t GetType(){
            return Type;
        }    
   
};

class TaskGoHome : public Task
{
    private:

    public:
        static const task_type_t Type = Task_Go_Home;

        void Setup(){

        }

         void Start(){

         }

        void Activate(){
           
        }

        bool Check(){
            return false;
        }

        task_type_t GetType(){
            return Type;
        }    
   
};

class TaskMowBoundary : public Task
{
    private:
    IMU* imu;
    float startYaw = 0;
    bool started = false;

    public:
        static const task_type_t Type = Task_Mow_Boundary;

        void Setup(IMU& imu){
            this->imu = &imu;
        }

        void Activate(){
           started = false;
        }

        void Start(){
            started = true;
            startYaw = abs(imu->GetYaw());
        }

        bool Check(){
            if(started){
                return abs(imu->GetYaw()) - startYaw > 630;
            }

            return false;
        }

        task_type_t GetType(){
            return Type;
        }    
   
};

class TaskPause : public Task
{
    private:

    public:
        static const task_type_t Type = Task_Sleep;

        void Setup(){

        }

         void Start(){

         }

        void Activate(){
           
        }

        bool Check(){
            return false;
        }

        task_type_t GetType(){
            return Type;
        }    
   
};

class TaskStop : public Task
{
    private:

    public:
        static const task_type_t Type = Task_Sleep;

        void Setup(){

        }

         void Start(){

         }

        void Activate(){
           
        }

        bool Check(){
            return false;
        }

        task_type_t GetType(){
            return Type;
        }    
   
};

