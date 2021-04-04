#pragma once
#include <Arduino.h>
#include "Gyro.h"

#define L3G4200D_ADDRESS           (0xD2 >> 1)

#define L3G4200D_REG_WHO_AM_I      (0x0F)

#define L3G4200D_REG_CTRL_REG1     (0x20)
#define L3G4200D_REG_CTRL_REG2     (0x21)
#define L3G4200D_REG_CTRL_REG3     (0x22)
#define L3G4200D_REG_CTRL_REG4     (0x23)
#define L3G4200D_REG_CTRL_REG5     (0x24)
#define L3G4200D_REG_REFERENCE     (0x25)
#define L3G4200D_REG_OUT_TEMP      (0x26)
#define L3G4200D_REG_STATUS_REG    (0x27)

#define L3G4200D_REG_OUT_X_L       (0x28)
#define L3G4200D_REG_OUT_X_H       (0x29)
#define L3G4200D_REG_OUT_Y_L       (0x2A)
#define L3G4200D_REG_OUT_Y_H       (0x2B)
#define L3G4200D_REG_OUT_Z_L       (0x2C)
#define L3G4200D_REG_OUT_Z_H       (0x2D)

#define L3G4200D_REG_FIFO_CTRL_REG (0x2E)
#define L3G4200D_REG_FIFO_SRC_REG  (0x2F)

#define L3G4200D_REG_INT1_CFG      (0x30)
#define L3G4200D_REG_INT1_SRC      (0x31)
#define L3G4200D_REG_INT1_THS_XH   (0x32)
#define L3G4200D_REG_INT1_THS_XL   (0x33)
#define L3G4200D_REG_INT1_THS_YH   (0x34)
#define L3G4200D_REG_INT1_THS_YL   (0x35)
#define L3G4200D_REG_INT1_THS_ZH   (0x36)
#define L3G4200D_REG_INT1_THS_ZL   (0x37)
#define L3G4200D_REG_INT1_DURATION (0x38)

typedef enum{
    L3G4200D_SCALE_250DPS  = 0b00,
    L3G4200D_SCALE_500DPS  = 0b01,
    L3G4200D_SCALE_2000DPS = 0b10
 

} l3g4200d_scale_t;

typedef enum{
    L3G4200D_DATARATE_800HZ_110  = 0b1111,
    L3G4200D_DATARATE_800HZ_50   = 0b1110,
    L3G4200D_DATARATE_800HZ_35   = 0b1101,
    L3G4200D_DATARATE_800HZ_30   = 0b1100,
    L3G4200D_DATARATE_400HZ_110  = 0b1011,
    L3G4200D_DATARATE_400HZ_50   = 0b1010,
    L3G4200D_DATARATE_400HZ_25   = 0b1001,
    L3G4200D_DATARATE_400HZ_20   = 0b1000,
    L3G4200D_DATARATE_200HZ_70   = 0b0111,
    L3G4200D_DATARATE_200HZ_50   = 0b0110,
    L3G4200D_DATARATE_200HZ_25   = 0b0101,
    L3G4200D_DATARATE_200HZ_12_5 = 0b0100,
    L3G4200D_DATARATE_100HZ_25   = 0b0001,
    L3G4200D_DATARATE_100HZ_12_5 = 0b0000
} l3g4200d_dataRate_t;

class L3G4200D : public Gyro
{

    private:
        l3g4200d_dataRate_t datarate;
        l3g4200d_scale_t scale;

        float dpsPerDigit = 0;

        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;

        int16_t tx = 0;
        int16_t ty = 0;
        int16_t tz = 0;

        int16_t bufferX = 0;
        int16_t bufferY = 0;
        int16_t bufferZ = 0;

        int16_t thresholdX = 0;
        int16_t thresholdY = 0;
        int16_t thresholdZ = 0;

        uint8_t counter = 0;


        int16_t offsetX = 0;
        int16_t offsetY = 0;
        int16_t offsetZ = 0;

        unsigned int intervall = 0;
        

        bool hasNewValues = false;

       
        void writeToRegister(uint8_t reg, uint8_t value);
        uint8_t readRegister(uint8_t reg);
        bool tryToFindGyro();
        void wakeUp();
        void goToSleep();

    public:
        void Setup(l3g4200d_scale_t scale, l3g4200d_dataRate_t datarate, uint16_t filterAlpha);
        void Sleep(bool sleeping);

        void UpdateValues();
        bool HasNewValues();

        void Calibrate(int samples = 200);

        void GetMDPSXYZ(int16_t& outX, int16_t& outY, int16_t& outZ);
        void GetDPSXYZ(float& outX, float& outY, float& outZ);


};
