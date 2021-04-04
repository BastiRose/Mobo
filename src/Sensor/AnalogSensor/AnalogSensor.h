#pragma once
#include "../../Component.h"

class AnalogSensor : public Component
{
    public:
        virtual void Update() = 0;
        virtual int GetValue() = 0;
};

