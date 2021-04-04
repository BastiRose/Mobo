#pragma once
#include <arduino.h>
#include "../../Component.h"

class BoundarySensor : public Component
{
    private:
        /* data */
    public:
        virtual bool IsActive() = 0;
        virtual bool IsInside() = 0;
        virtual bool IsNear() = 0;
        virtual int BoundaryError() = 0;
        virtual int BoundaryErrorMax() = 0;
        virtual unsigned long TimeOutside() = 0;
};

