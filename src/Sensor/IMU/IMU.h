#pragma once
#include <Arduino.h>

class IMU
{
    private:
        
    public:

    virtual void Update() = 0;

    virtual void SetHeading() = 0;
    virtual float GetHeadingError() = 0;

    virtual float GetRoll() = 0;
    virtual float GetPitch() = 0;
    virtual float GetYaw() = 0;




};

