#pragma once
#include "BoundarySensor.h"
#include "perimeter.h"

class WireSensor : public BoundarySensor{
    private:
        Perimeter sensor;
        unsigned long wireOffTimer = 0;
        unsigned long magUpdateTimer = 0;

        unsigned long outsideTimer = 0;

         unsigned long lastTimePassedBoundar = 0;

        int mag = 0;
        int maxMag = 0;

        bool calibrated = false;

        bool isNear = false;
        bool wasOverMax = false;

        bool wireActive = false;

        bool isInside = false;
        bool lastInside = false;

    public:
        void Setup(byte pin);

        void Update(uint32_t now);

        bool IsActive();
        bool IsInside();
        bool IsNear();
        int BoundaryError();
        int BoundaryErrorMax();
        unsigned long TimeOutside();
        unsigned long LastTimePassedBoundary();
};

