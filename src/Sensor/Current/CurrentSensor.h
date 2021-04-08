#pragma once
#include <arduino.h>
#include "../../Component.h"

class CurrentSensor : public Component
{
    
    public:
        virtual void Update(uint32_t now) = 0;
        virtual unsigned int GetCurrent() = 0;
        virtual unsigned int GetPower(int voltage) = 0;
};

