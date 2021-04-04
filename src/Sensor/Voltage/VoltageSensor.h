#pragma once
#include <arduino.h>
#include "../../Component.h"

class VoltageSensor : public Component
{
    public:
        virtual void Update() = 0;
        virtual long GetMillivoltage() = 0;
};