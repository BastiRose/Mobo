#pragma once
#include <Arduino.h>
#include "Motor.h"
#include "Helper.h"
#include "Sensor/IMU/IMU.h"

class MovementAction{
protected:
    bool done = false;
    bool isReady = false;
    bool isActive = false;

    unsigned int id = 0;

    Motor* motorLeft;
    Motor* motorRight;
    

public:
    MovementAction(){
        id = GetUniqueNumber();
    }

    bool IsDone(){
        return done;
    }

    void Init(Motor* motorLeft, Motor* motorRight){
        isReady = true;

        this->motorLeft = motorLeft;
        this->motorRight = motorRight;
    }

    bool IsActive(){
        return isActive;
    }

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void Execute() = 0;
    virtual void ForceDone() = 0;

    unsigned int GetID(){ return id;}

    

    inline bool operator==(const MovementAction& other){
        return this->id == other.id;
    }

};

/*####### ACTIONS #######*/

class MovementActionBreak : public MovementAction {

protected:
    unsigned long timer = 0;
    unsigned int breakTime = 0;
public:
    void SetDuration(unsigned int breakTime){
        this->breakTime = breakTime;
    }

    void Activate(){
        done = false;
        isActive = true;
        timer = millis();
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }

    void Execute(){
        if(!isReady)
            return;

        if(!motorLeft->IsBraking() || !motorRight->IsBraking()){ 
            motorLeft->Brake();
            motorRight->Brake();
        }

        if(breakTime != 0 && millis() - timer >= breakTime){
            ForceDone();
        }
    }

    void ForceDone(){
        done = true;
    }
};

class MovementActionStop : public MovementAction {

protected:
    unsigned long timer = 0;
    unsigned int stopTime = 0;
    bool waitForMotorStoped = false;
public:

    void SetDuration(unsigned int stopTime){
        this->stopTime = stopTime;
    }

    void SetWaitForMotorStoped(bool waitForMotorStoped){
        this->waitForMotorStoped = waitForMotorStoped;
    }

    void Activate(){
        done = false;
        isActive = true;
        timer = millis();
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }

    void Execute(){
        if(!isReady)
            return;

        if(motorLeft->GetTargetSpeed() != 0 || motorRight->GetTargetSpeed() != 0){ 
            motorLeft->ChangeSpeed(0);
            motorRight->ChangeSpeed(0);
        }

        if(waitForMotorStoped && ( motorLeft->GetSpeed() != 0 || motorRight->GetSpeed() != 0)){
            return;
        }

        if(stopTime != 0 && millis() - timer >= stopTime){
            ForceDone();
        }
    }

    void ForceDone(){
        done = true;
    }
};

class MovementActionForward : public MovementAction {

protected:
    int maxSpeed = 0;
    int speedL = 0;
    int speedR = 0;

    int speed = 0;

    float pOut = 0;
    float iOut = 0;
    float errorSum = 0;
    float dt = 0;
    bool usePID = true;

    float kp = 28;
    float ki = 18;

    unsigned long timer = 0;

    IMU* imu;

public:
    void Setup(int speed, IMU& imu){
        this->maxSpeed = speed;
        this->imu = &imu;
    }

    void ChangeSpeed(int speed){
        this->maxSpeed = speed;
    }

    void Activate(){
        done = false;
        isActive = true;
        imu->SetHeading();
        speedL = 0;
        speedR = 0;
        speed = 0;
        timer = millis();
        pOut = 0;
        iOut = 0;
        errorSum= 0;
        dt = 10.0/1000.0;
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }
    void UsePID(bool usePID){
        this->usePID = usePID;
    }

    void SetHeading(){
        imu->SetHeading();
    }

    void Execute(){
        if(!isReady)
            return;

        if(millis() - timer > 10){
            timer = millis();

            if(usePID){
                pOut = (imu->GetHeadingError() * kp);
               
                errorSum += imu->GetHeadingError();
                iOut =  ki * errorSum * dt;

                speed = maxSpeed;
                
                if(iOut < 0){
                    iOut = 0;
                    errorSum = 0;
                }

                if(iOut > speed){
                    iOut = speed;
                    errorSum = speed / dt / ki;
                }
                
                if(imu->GetHeadingError() > 0){
                    speedL = maxSpeed - (pOut + iOut);
                    speedR = maxSpeed;
                } else {
                    speedL = maxSpeed;
                    speedR = maxSpeed + (pOut + iOut);
                }

                if(speedL > maxSpeed)
                    speedL = maxSpeed;

                if(speedL < 0)
                    speedL = 0;

                if(speedR > maxSpeed)
                    speedR = maxSpeed;

                if(speedR < 0)
                    speedR = 0;

                if(motorLeft->GetTargetSpeed() != speedL){
                    motorLeft->ChangeSpeed(speedL, 0);
                }

                if(motorRight->GetTargetSpeed() != speedR){
                    motorRight->ChangeSpeed(speedR, 0);
                }

            } else {
                speedL = maxSpeed;
                speedR = maxSpeed;

                if(motorLeft->GetTargetSpeed() != speedL){
                    motorLeft->ChangeSpeed(speedL);
                }

                if(motorRight->GetTargetSpeed() != speedR){
                    motorRight->ChangeSpeed(speedR);
                }
            }
        }
    }

    void ForceDone(){
        done = true;
    }
};

class MovementActionBackwards : public MovementAction {

protected:
    int maxSpeed = 0;
    int speedL = 0;
    int speedR = 0;

    IMU* imu;

public:
    void Setup(int speed, IMU& imu){
        this->maxSpeed = speed;
        this->imu = &imu;
    }

    void ChangeSpeed(int speed){
        this->maxSpeed = speed;
    }

    void Activate(){
        done = false;
        isActive = true;
        imu->SetHeading();
        speedL = 0;
        speedR = 0;
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }

    void Execute(){
        if(!isReady)
            return;
        speedR = maxSpeed;
        speedL = maxSpeed;

        if(motorLeft->GetTargetSpeed() != -speedL){
            motorLeft->ChangeSpeed(-speedL);
        }

        if(motorRight->GetTargetSpeed() != -speedR){
            motorRight->ChangeSpeed(-speedR);
        } 
    }

    void ForceDone(){
        done = true;
    }
};

class MovementActionCurve : public MovementAction {

protected:
    protected:
    int maxSpeed = 0;
    int speedL = 0;
    int speedR = 0;

    int speed = 0;

    int target = 0;

    float currentTarget = 0;
    float degreePerTick = 0;


    float pOut = 0;
    float iOut = 0;
    float error = 0;
    float errorSum = 0;
    float dt = 0;

    bool usePID = true;

    float kp = 28;
    float ki = 18;

    unsigned long timer = 0;

    IMU* imu;

public:
    void Setup(int speed, IMU& imu){
        this->maxSpeed = speed;
        this->imu = &imu;
    }

    void SetTarget(int target){
        this->target = target;
    }

    void SetDegPerSec(float degreePerSec){
        this->degreePerTick = degreePerSec / 100.0;
    }

    void ChangeSpeed(int speed){
        this->maxSpeed = speed;
    }

    void Activate(){
        done = false;
        isActive = true;
        imu->SetHeading();
        speedL = 0;
        speedR = 0;
        speed = 0;
        timer = millis();
        pOut = 0;
        iOut = 0;
        errorSum= 0;
        dt = 10.0/1000.0;
        currentTarget = 0;
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }
    void UsePID(bool usePID){
        this->usePID = usePID;
    }

    void SetHeading(){
        imu->SetHeading();
    }

    void Execute(){
        if(!isReady)
            return;

        if(millis() - timer > 10){
            timer = millis();

            if(target != 0){
                if(currentTarget < target){
                    currentTarget += degreePerTick;
                } else {
                    if(currentTarget > target){
                        currentTarget -= degreePerTick;
                    }
                }

                if(abs(imu->GetHeadingError()) >= target){
                    done = true;
                }

            } else {
                currentTarget += degreePerTick;
            }

            if(usePID){

                error = (currentTarget - imu->GetHeadingError());  

                pOut = (error * kp);
                errorSum += error;
                iOut =  ki * errorSum * dt;

                speed = maxSpeed;
                
                if(iOut < 0){
                    iOut = 0;
                    errorSum = 0;
                }

                if(iOut > speed){
                    iOut = speed;
                    errorSum = speed / dt / ki;
                }
                
                if(imu->GetHeadingError() > 0){
                    speedL = maxSpeed - (pOut + iOut);
                    speedR = maxSpeed;
                } else {
                    speedL = maxSpeed;
                    speedR = maxSpeed + (pOut + iOut);
                }

                if(speedL > maxSpeed)
                    speedL = maxSpeed;

                if(speedL < 0)
                    speedL = 0;

                if(speedR > maxSpeed)
                    speedR = maxSpeed;

                if(speedR < 0)
                    speedR = 0;

                if(motorLeft->GetTargetSpeed() != speedL){
                    motorLeft->ChangeSpeed(speedL, 0);
                }

                if(motorRight->GetTargetSpeed() != speedR){
                    motorRight->ChangeSpeed(speedR, 0);
                }

            } else {
                speedL = maxSpeed;
                speedR = maxSpeed;

                if(motorLeft->GetTargetSpeed() != speedL){
                    motorLeft->ChangeSpeed(speedL);
                }

                if(motorRight->GetTargetSpeed() != speedR){
                    motorRight->ChangeSpeed(speedR);
                }
            }
        }
    }

    void ForceDone(){
        done = true;
    }
};

class MovementActionTurnOnPoint : public MovementAction {

protected:
    IMU* imu;

    int speed = 0;
    int currentSpeed = 0;

    float wantedTarged = 0;
    float targed = 0;

    int8_t dir = 1;

    float pOut = 0;
    float iOut = 0;
    float dOut = 0;
    float errorSum = 0;

    float kp = 14;
    float ki = 5.5;
    float kd = 0.5;

    float error = 0;
    float lastError = 0;

    float allowedError = 3;

    float dt = 0;

    unsigned long timer = 0;

    bool exact = false;
    bool usePID = false;

    int counter = 0;

    void turnWithIMU(){
        pOut = kp * error;

        errorSum += error;
        iOut =  ki * errorSum * dt;

        if(iOut < -speed){
            iOut = -speed;
            errorSum = -speed / dt / ki;
        }

        if(iOut > speed){
            iOut = speed;
            errorSum = speed / dt / ki;
        }

        dOut = kd / dt *  (error - lastError) ;

        currentSpeed = (pOut + iOut + dOut);

        if(currentSpeed > speed)
            currentSpeed = speed;
                
        if(currentSpeed < -speed)
             currentSpeed = -speed;

        if(motorRight->GetTargetSpeed() != -currentSpeed  || motorLeft->GetTargetSpeed() != currentSpeed ){
            motorLeft->ChangeSpeed(currentSpeed, 0);
            motorRight->ChangeSpeed(-currentSpeed, 0);
        }
    }

    void turn(){
        currentSpeed = (speed - (speed / 2) * (abs(targed) / abs(imu->GetHeadingError()))) * dir;
    
        if((dir > 0 && imu->GetHeadingError() >= targed) || (dir < 0 && imu->GetHeadingError() <= -targed)){
            done = true;
            motorLeft->ChangeSpeed(0);
            motorRight->ChangeSpeed(0);
            return;
        }

        if(motorRight->GetTargetSpeed() != -currentSpeed  || motorLeft->GetTargetSpeed() != currentSpeed ){
             motorLeft->ChangeSpeed(currentSpeed);
            motorRight->ChangeSpeed(-currentSpeed);
        }
    }

    void checkForStuckOrDone(){
        if(abs(error - lastError) < 0.2 && (!exact || abs(error) <= allowedError) ){
            counter++;
            if(counter >= 100 || (!exact && abs(error) <= allowedError)){
                done = true; 
                motorLeft->ChangeSpeed(0);
                motorRight->ChangeSpeed(0);        
                return;
            }
        } else {
            counter = 0;
        }
    }

public:
    void Setup(int speed, IMU& imu){
        this->speed = speed * dir;
        this->imu = &imu;
    }

    void SetTargetAngle(int angle){
        counter = 0;
        this->wantedTarged = angle;
    }

    void EnablePID(bool usePID, bool exact){
        this->usePID = usePID;
        this->exact = exact;
        if(usePID == false){
            usePID = false;
            exact = false;
        }
    }

    void SetHeading(){
        imu->SetHeading();
    }

    void SetDirection(int8_t dir){
        if(dir == 0){
            if(random(0, 2) == 0)
                dir = 1;
            else 
                dir = -1;
        }
        this->dir = dir;
    }

    void Activate(){
        done = false;
        isActive = true;
        imu->SetHeading();

        timer = millis();

        iOut= 0;
        pOut = 0;
        dOut = 0;

        errorSum = 0;

        error = targed; 
        lastError = 0;
        dt = 0;

        counter = 0;
        targed = 0;

        currentSpeed = 0;
    }

    void Deactivate(){
        done = true;
        isActive = false;
    }

    void Execute(){
        if(!isReady || done)
            return;

        if(millis() - timer >= 10){
            
            if(wantedTarged * dir < targed)
                targed -= 0.5;
            else
                if(wantedTarged * dir > targed)
                    targed += 0.5;

            error = (targed) - imu->GetHeadingError(); 
            dt = (millis() - timer) / 1000.0;
            timer = millis();

            checkForStuckOrDone();

            if(usePID){
                turnWithIMU();
            } else {
                turn();
            } 
            lastError = error;
        }
    }

    int AngleTurned(){
        if(done)
           return targed;
        return (imu->GetHeadingError());
    }
    void ForceDone(){
        done = true;
    }
};