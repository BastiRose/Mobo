#include "ADXL345.h"
#include <Wire.h>

void ADXL345::Setup(adxl345_scale_t scale, adxl345_dataRate_t dataRate){

    this->dataRate = dataRate;
    this->scale = scale;

    if(!tryToFindACC()){
        Serial.println("ACC ADXL345 not connected!");
        return;
    }

    Serial.println("ACC ADXL345 found!");

    wakeUp();
}


bool ADXL345::tryToFindACC(){

    uint8_t id = 0;
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXL345_REG_DEVID);
    Wire.endTransmission(true);

    Wire.requestFrom(ADXL345_ADDRESS, 1);
    
    while (!Wire.available());
    id = Wire.read();


    return id == 0xE5;
}

void ADXL345::wakeUp(){
    writeToRegister(ADXL345_REG_BW_RATE, 0x00 | dataRate); // Set Data range
    writeToRegister(ADXL345_REG_DATA_FORMAT, 0b00001000 | scale); //Set Full_Res and Range
    writeToRegister(ADXL345_REG_POWER_CTL, 0b00101000); // Measure mode
    
}

void ADXL345::Calibrate(int samples = 200){
    int16_t sumX = 0;
    int16_t sumY = 0;
    int16_t sumZ = 0;

    int16_t maxX = 0;
    int16_t maxY = 0;
    int16_t maxZ = 0;

    int realSamples = 0;

    
    writeToRegister(ADXL345_REG_OFSX, 0);
    writeToRegister(ADXL345_REG_OFSY, 0);
    writeToRegister(ADXL345_REG_OFSZ, 0);

    delay(10);

    for(int i = 0; i < samples; i++){
        UpdateValues();
        sumX += tx;
        sumY += ty;
        sumZ += tz;

        delay(10);
    }


    x = 0;
    y = 0;
    z = 0;

    writeToRegister(ADXL345_REG_OFSX,-round((sumX / samples) / 3.9));
    writeToRegister(ADXL345_REG_OFSY,-round((sumY / samples) / 3.9));
    writeToRegister(ADXL345_REG_OFSZ,-round(((sumZ / samples) - 256) / 3.9));


    Serial.print("ACC Offset: X: ");
    Serial.print(-round((sumX / samples) / 3.9));
    Serial.print(" Y: ");
    Serial.print(-round((sumY / samples) / 3.9));
    Serial.print(" Z: ");
    Serial.println(-round(((sumZ / samples)- 256) / 3.9));

}

void ADXL345::UpdateValues(){
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXL345_REG_DATAX0 | (1 << 7)); // Set MSB to 1 to read multiple bytes Starting from DATAX0
    Wire.endTransmission(true);

    Wire.requestFrom(ADXL345_ADDRESS, 6);
    
    tx = Wire.read(); 
    tx = ((Wire.read()) << 8) | tx;

    ty = Wire.read(); 
    ty = ((Wire.read()) << 8) | ty;

    tz = Wire.read(); 
    tz = ((Wire.read()) << 8) | tz;

    bufferX += (tx >= 0 ? tx >> 2 : -(abs(tx) >> 2));
    bufferY += (ty >= 0 ? ty >> 2 : -(abs(ty) >> 2));
    bufferZ += (tz >= 0 ? tz >> 2 : -(abs(tz) >> 2));

    count++;

    if(count >= 4){
        count = 0;

        x = bufferX;
        y = bufferY;
        z = bufferZ;

        bufferX = 0;
        bufferY = 0;
        bufferZ = 0;

        hasNewValues = true;
    }
}

bool ADXL345::HasNewValues(){
    return hasNewValues;
}

void ADXL345::GetMGXYZ(int16_t& outX, int16_t& outY, int16_t& outZ){
    outX = x >> 2;
    outY = y >> 2;
    outZ = z >> 2;

    hasNewValues = false;
}

void ADXL345::GetGXYZ(float& outX, float& outY, float& outZ){
    outX = x * 0.0039;
    outY = y * 0.0039;
    outZ = z * 0.0039;

    hasNewValues = false;
}

void ADXL345::GetAngle(float& outRoll, float& outPitch){
    if((x == 0 && z == 0) || y == 0)
        outRoll = 0;
    else
        outRoll = (atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI) ;

    if((x == 0 && z == 0) || x == 0)
        outPitch == 0;
    else 
        outPitch = (atan(x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI);
}

void ADXL345::writeToRegister(uint8_t reg, uint8_t value){
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}