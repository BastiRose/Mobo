
#pragma once
#include <arduino.h>
#include "../../Component.h"


class DistanceSensor : public Component
{
    public:
        virtual void Update(uint32_t now) = 0;
        virtual unsigned int GetDistance() = 0;
};

