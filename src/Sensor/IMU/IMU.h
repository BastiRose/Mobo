#pragma once
#include <Arduino.h>
#include "../../Component.h"

class IMU: public Component
{
    private:
        
    public:

    virtual void Update(uint32_t now) = 0;

    virtual void SetHeading() = 0;
    virtual float GetHeadingError() = 0;

    virtual float GetRoll() = 0;
    virtual float GetPitch() = 0;
    virtual float GetYaw() = 0;

};

