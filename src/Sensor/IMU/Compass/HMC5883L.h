#pragma once
#include "Compass.h"
#include <Arduino.h>

#define HMC5883L_ADDRESS        0x1E
#define HMC5883L_IDA            0x48
#define HMC5883L_IDB            0x34
#define HMC5883L_IDC            0x33

#define HMC5883L_REG_CONFIG_A   0x0
#define HMC5883L_REG_CONFIG_B   0x1
#define HMC5883L_REG_MODE       0x2
#define HMC5883L_REG_Out_X_MSB  0x3
#define HMC5883L_REG_Out_X_LSB  0x4
#define HMC5883L_REG_Out_Y_MSB  0x5
#define HMC5883L_REG_Out_Y_LSB  0x6
#define HMC5883L_REG_Out_Z_MSB  0x7
#define HMC5883L_REG_Out_Z_LSB  0x8
#define HMC5883L_REG_STATUS     0x9
#define HMC5883L_REG_ID_A       0xA
#define HMC5883L_REG_ID_B       0xB
#define HMC5883L_REG_ID_C       0xC

typedef enum{
    HMC5883L_SAMPLES_1 = 0b00,
    HMC5883L_SAMPLES_2 = 0b01,
    HMC5883L_SAMPLES_4 = 0b10,
    HMC5883L_SAMPLES_8 = 0b11
} hmc588l_sample_count_t;

typedef enum{
    HMC5883L_RATE_0_75  =   0b000,
    HMC5883L_RATE_1_5   =   0b001,
    HMC5883L_RATE_3     =   0b010,
    HMC5883L_RATE_7_5   =   0b011,
    HMC5883L_RATE_15    =   0b100,
    HMC5883L_RATE_30    =   0b101,
    HMC5883L_RATE_75    =   0b110
} hmc588l_rate_t;

typedef enum{
    HMC5883L_RANGE_0_88  =   0b000,
    HMC5883L_RANGE_1_3   =   0b001,
    HMC5883L_RANGE_1_9   =   0b010,
    HMC5883L_RANGE_2_5   =   0b011,
    HMC5883L_RANGE_4     =   0b100,
    HMC5883L_RANGE_4_7   =   0b101,
    HMC5883L_RANGE_5_6   =   0b110,
    HMC5883L_RANGE_8_1   =   0b111
} hmc588l_range_t;

class HMC5883L : public Compass
{
    private:
        hmc588l_sample_count_t smapleCount;
        hmc588l_rate_t rate;
        hmc588l_range_t range;

        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;

        int16_t tx = 0;
        int16_t ty = 0;
        int16_t tz = 0;

        float offsetX = 0;
        float offsetY = 0;
        float offsetZ = 0;

        float mgPerDigit = 0;

        bool tryToFindMag();
        void writeToRegister(uint8_t reg, uint8_t value);

        void wakeUp();
    public:
        void Setup(hmc588l_sample_count_t smapleCount, hmc588l_rate_t rate, hmc588l_range_t range);
        void UpdateValues();
        void Calibrate(int samples = 200);
        void GetGaus(float& outX, float& outY, float& outZ);
        float GetAngle();
    
};

