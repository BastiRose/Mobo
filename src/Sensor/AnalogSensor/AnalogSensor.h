#pragma once
#include "../../Component.h"

class AnalogSensor : public Component
{
    public:
        virtual void Update(uint32_t now) = 0;
        virtual int GetValue() = 0;
};

