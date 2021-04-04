#pragma once

class Gyro
{
    protected:

    public:
        
        virtual void UpdateValues() = 0;
        virtual bool HasNewValues() = 0;

        virtual void GetMDPSXYZ(int16_t& outX, int16_t& outY, int16_t& outZ) = 0;
        virtual void GetDPSXYZ(float& outX, float& outY, float& outZ) = 0;


        virtual void Calibrate(int samples) = 0;
};

