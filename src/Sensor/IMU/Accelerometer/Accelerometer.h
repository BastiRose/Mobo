#pragma once
#include <Arduino.h>

class Accelerometer
{
    private:
        /* data */
    public:
        virtual void UpdateValues() = 0;
        virtual bool HasNewValues() = 0;
        virtual void Calibrate(int samples = 200) = 0;
        virtual void GetMGXYZ(int16_t& outX, int16_t& outY, int16_t& outZ) = 0;
        virtual void GetGXYZ(float& outX, float& outY, float& outZ) = 0;
        virtual void GetAngle(float& outX, float& outY) = 0;

};

