#include "L3G4200D.h"
#include <Wire.h>

void L3G4200D::Setup(l3g4200d_scale_t scale, l3g4200d_dataRate_t datarate, uint16_t filterAlpha){
    this->datarate = datarate;
    this->scale = scale;
   
    if(!tryToFindGyro()){
        Serial.println("Gyro L3G4200D not connected!");
        return;
    }
    Serial.println("Gyro L3G4200D found!");

    wakeUp();

    writeToRegister(L3G4200D_REG_CTRL_REG2, 0x00); //Set High Pass filter to normal mode
    writeToRegister(L3G4200D_REG_CTRL_REG3, 0x00); //No interrupts
    writeToRegister(L3G4200D_REG_CTRL_REG4, 0b10000000 | scale << 4 ); //Block mode and set scale

    if(scale == L3G4200D_SCALE_250DPS)
        dpsPerDigit = 8.75 / 1000.0;
    if(scale == L3G4200D_SCALE_500DPS)
        dpsPerDigit = 17.50 / 1000.0;
    if(scale == L3G4200D_SCALE_2000DPS)
        dpsPerDigit = 70.00 / 1000.0;
}


void L3G4200D::UpdateValues(){
    
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_REG_OUT_X_L | (1 << 7)); // Set MSB to 1 to read multiple bytes Starting from Out_X_L
    Wire.endTransmission(true);

    Wire.requestFrom(L3G4200D_ADDRESS, 6);
    
    tx = Wire.read(); 
    tx = ((Wire.read()) << 8) | tx;

    ty = Wire.read(); 
    ty = ((Wire.read()) << 8) | ty;

    tz = Wire.read(); 
    tz = ((Wire.read()) << 8) | tz;

    tx -= offsetX;
    ty -= offsetY;
    tz -= offsetZ;

    bufferX += (tx >= 0 ? tx >> 2 : -(abs(tx) >> 2));
    bufferY += (ty >= 0 ? ty >> 2 : -(abs(ty) >> 2));
    bufferZ += (tz >= 0 ? tz >> 2 : -(abs(tz) >> 2));

    counter++;
    if(counter >= 4){   
        counter = 0;

        x = 0;
        y = 0;
        z = 0;

        if(abs(bufferX) >= thresholdX)
            x = bufferX;   

        if(abs(bufferY) >= thresholdY)
            y = bufferY;
        
        if(abs(bufferZ) >= thresholdZ)
            z = bufferZ;

        bufferX = 0;
        bufferY = 0;
        bufferZ = 0;
        hasNewValues = true;
    }
}

bool L3G4200D::HasNewValues(){
    return hasNewValues;
}

void L3G4200D::Calibrate(int samples = 200){
    int16_t sumX = 0;
    int16_t sumY = 0;
    int16_t sumZ = 0;

    int16_t maxX = 0;
    int16_t maxY = 0;
    int16_t maxZ = 0;

    int realSamples = 0;

    for(int i = 0; i < samples; i++){
        UpdateValues();
        sumX += tx;
        sumY += ty;
        sumZ += tz;

        if(abs(tx) > thresholdX)
            thresholdX = abs(tx);
        
        if(abs(ty) > thresholdY)
            thresholdY = abs(ty);
        
        if(abs(tz) > thresholdZ)
            thresholdZ = abs(tz);

        delay(10);
    }

    offsetX = sumX / samples;
    offsetY = sumY / samples;
    offsetZ = sumZ / samples;

    thresholdX /= 2;
    thresholdY /= 2;
    thresholdZ /= 2;

    x = 0;
    y = 0;
    z = 0;

    bufferX = 0;
    bufferY = 0;
    bufferZ = 0;

    Serial.print("Gyro Offset: X: ");
    Serial.print(offsetX);
    Serial.print(" Y: ");
    Serial.print(offsetY);
    Serial.print(" Z: ");
     Serial.println(offsetZ);

    Serial.print("Gyro Threshold: X: ");
    Serial.print(thresholdX);
    Serial.print(" Y: ");
    Serial.print(thresholdY);
    Serial.print(" Z: ");
    Serial.println(thresholdZ);

}

bool L3G4200D::tryToFindGyro(){

    uint8_t id = 0;
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(L3G4200D_REG_WHO_AM_I);
    Wire.endTransmission(true);

    Wire.requestFrom(L3G4200D_ADDRESS, 1);
    
    while (!Wire.available());
    id = Wire.read();


    return id == 0xD3;
}

void L3G4200D::wakeUp(){
    //Set to normal mode and the datarate [DR1|DR2|BW1|BE2|PD|Zen|Xen|Yen]
    uint8_t ctlr_reg1 = 0x00;
    ctlr_reg1 |= 0x0F; // Enables xyz (normal mode)
    ctlr_reg1 |= (datarate << 4); // set datarate and cut off 

   writeToRegister(L3G4200D_REG_CTRL_REG1, ctlr_reg1);
}


void L3G4200D::goToSleep(){
    //Set to sleep mode and the datarate [DR1|DR2|BW1|BE2|PD|Zen|Xen|Yen]
    uint8_t ctlr_reg1 = 0x08;
    ctlr_reg1 |= (datarate << 4); // set datarate and cut off 

    writeToRegister(L3G4200D_REG_CTRL_REG1, ctlr_reg1);
}

void L3G4200D::writeToRegister(uint8_t reg, uint8_t value){
    Wire.beginTransmission(L3G4200D_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}

void L3G4200D::GetMDPSXYZ(int16_t& outX, int16_t& outY, int16_t& outZ){
    outX = round((x * dpsPerDigit) * 1000);
    outY = round((y * dpsPerDigit) * 1000);
    outZ = round((z * dpsPerDigit) * 1000);
    hasNewValues = false;
}

void L3G4200D::GetDPSXYZ(float& outX, float& outY, float& outZ){
    outX = x * dpsPerDigit;
    outY = y * dpsPerDigit;
    outZ = z * dpsPerDigit;
    hasNewValues = false;
}







