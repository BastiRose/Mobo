#pragma once
#include "MovementSystem.h"


class  Motor;

class MovementSystemTimeBased : public MovementSystem{
    private:
        int targetDistance = 0;
        int goalAngle = 0;
        int targetAngle = 0;
        int targetSpeed = 0;

        unsigned int smoothnes = 0;
        int angle = 0;
        int distance = 0;

        unsigned int milliDegreePerMillimeter = 0;

        unsigned int milliDegreePerSec = 5;
        unsigned int millimeterPerSec = 20;

        unsigned long timeToReachTargetDistance = 0;
        int motorSpeedForSlowerMotor = 0;

        unsigned long timerDistance = 0;
        unsigned int intervallDistanceUpdate = 0;
        unsigned int cIntervallDistanceUpdate = 0;

        unsigned long timerDegree = 0;
        unsigned int intervallDegreeUpdate = 0;
        unsigned int cIntervallDegreeUpdate = 0;

        unsigned long driftTimer = 0;
        int angleError = 0;
        int millimerterDegreePerUpdateDrift = 0;


        Motor* motorLeft;
        Motor* motorRight;

        byte state = 0;

        void doForward();
        void doBackwards();
        void doCurve();
        void doTrunOnPoint();
    public:
        void Setup(Motor& motorLeft, Motor& motorRight, unsigned int milliDegreePerSec, unsigned int millimeterPerSec);

        void MoveForward(int speed, int distance, unsigned int smoothness);
        void MoveBack(int speed, int distance, unsigned int smoothness);

        void TurnOnPoint(int speed, int targetAngle, unsigned int smoothness);

        void MoveCurve(int speed, int degreePerSec, int distance, unsigned int smoothness);

        void Stop(unsigned int smoothness);
        void Brake();

        void Update();
        byte GetProgress();
};

