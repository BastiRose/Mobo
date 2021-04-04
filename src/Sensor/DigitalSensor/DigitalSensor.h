#pragma once
#include "../../Component.h"
class DigitalSensor : public Component
{
    public:
        virtual void Update() = 0;
        virtual bool IsActive() = 0;
};


