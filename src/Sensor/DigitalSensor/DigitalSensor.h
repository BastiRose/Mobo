#pragma once
#include "../../Component.h"
class DigitalSensor : public Component
{
    public:
        virtual void Update(uint32_t now) = 0;
        virtual bool IsActive() = 0;
};


