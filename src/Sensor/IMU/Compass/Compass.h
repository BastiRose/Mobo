#pragma once

class Compass
{
    private:
        /* data */
    public:
        virtual void UpdateValues() = 0;
        virtual void Calibrate(int samples = 200) = 0;
        virtual void GetGaus(float& outX, float& outY, float& outZ) = 0;
        virtual float GetAngle() = 0;
};

