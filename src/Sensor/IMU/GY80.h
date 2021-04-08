#pragma once
#include "IMU.h"
#include "Gyro/L3G4200D.h"
#include "Accelerometer/ADXL345.h"

class GY80 : public IMU
{
private:
    L3G4200D gyro;
    ADXL345 acc;

    float ax = 0;
    float ay = 0;
    float az = 0;

    float ar = 0;
    float ap = 0;

    float gx = 0;
    float gy = 0;
    float gz = 0;

    float yaw = 0;
    float roll = 0;
    float pitch = 0;

    float aLength = 0;
    float dotProductAG = 0;

    float dt = 0;

    unsigned long timer1 = 0;
    unsigned long timer2 = 0;

    uint8_t counter = 0;

    float headingYaw = 0;

public:
    void Setup();
    void Update(uint32_t now);

    void SetHeading();
    float GetHeadingError();

    float GetRoll();
    float GetPitch();
    float GetYaw();
};

