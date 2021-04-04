#include "HMC5883L.h"
#include <Wire.h>

void HMC5883L::Setup(hmc588l_sample_count_t smapleCount, hmc588l_rate_t rate, hmc588l_range_t range){
    this->smapleCount = smapleCount;
    this->rate = rate;
    this->range = range;

    if(!tryToFindMag()){
        Serial.println("Compass HMC5883L not connected!");
        return;
    }

    Serial.println("Compass HMC5883L found!");

        switch(range)
    {
	case HMC5883L_RANGE_0_88:
	    mgPerDigit = 0.073f / 1000.0;
	    break;

	case HMC5883L_RANGE_1_3:
	    mgPerDigit = 0.92f / 1000.0;
	    break;

	case HMC5883L_RANGE_1_9:
	    mgPerDigit = 1.22f / 1000.0;
	    break;

	case HMC5883L_RANGE_2_5:
	    mgPerDigit = 1.52f / 1000.0;
	    break;

	case HMC5883L_RANGE_4:
	    mgPerDigit = 2.27f / 1000.0;
	    break;

	case HMC5883L_RANGE_4_7:
	    mgPerDigit = 2.56f / 1000.0;
	    break;

	case HMC5883L_RANGE_5_6:
	    mgPerDigit = 3.03f / 1000.0;
	    break;

	case HMC5883L_RANGE_8_1:
	    mgPerDigit = 4.35f / 1000.0;
	    break;

	default:
	    break;
    }

    wakeUp();
}


bool HMC5883L::tryToFindMag(){

    uint8_t idA = 0;
    uint8_t idB = 0;
    uint8_t idC = 0;

    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(HMC5883L_REG_ID_A);
    Wire.endTransmission(true);

    Wire.requestFrom(HMC5883L_ADDRESS, 3);

    idA = Wire.read();
    idB = Wire.read();
    idC = Wire.read();

    return (idA == HMC5883L_IDA && idB == HMC5883L_IDB && idC == HMC5883L_IDC);
}

void HMC5883L::wakeUp(){
    writeToRegister(HMC5883L_REG_CONFIG_A, 0x00 | (smapleCount << 5) | (rate << 2));
    writeToRegister(HMC5883L_REG_CONFIG_B, 0x00 | (range << 5));
    writeToRegister(HMC5883L_REG_MODE, 0);   
}

void HMC5883L::Calibrate(int samples = 200){

  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x00);
  Wire.write(0b01111000); 


  Wire.endTransmission();
  
   Serial.println("Calibrating the Magnetometer ....... Offset");
    Serial.println("Please rotate the magnetometer 2 or 3 times in complete circules with in one minute ............. ");
    
    for(byte i=0;i<10;i++){   // Disregarding first few data
         UpdateValues();
     }
    
    float x_max=-4000,y_max=-4000,z_max=-4000; 
    float x_min=4000,y_min=4000,z_min=4000;
    
    
    unsigned long t = millis();
    while(millis()-t <= 30000){
      
      UpdateValues();
      float sx, sy, sz;
      GetGaus(sx, sy, sz);


      x_max = max(x_max,sx);
      y_max = max(y_max,sy);
      z_max = max(z_max,sz);
  
      
      x_min = min(x_min,sx);
      y_min = min(y_min,sy);
      z_min = min(z_min,sz);
    }
    
    offsetX = ((x_max-x_min)/2)-x_max;
    offsetY = ((y_max-y_min)/2)-y_max;
    offsetZ = ((z_max-z_min)/2)-z_max;
    
    Serial.print("Offset x  = ");
    Serial.print(offsetX);
    Serial.println(" mG");
    Serial.print("Offset y  = ");
    Serial.print(offsetY);
    Serial.println(" mG");
    Serial.print("Offset z  = ");
    Serial.print(offsetZ);
    Serial.println(" mG");
}

void HMC5883L::UpdateValues(){
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(HMC5883L_REG_Out_X_LSB);
    Wire.endTransmission(true);

    Wire.requestFrom(HMC5883L_ADDRESS, 6);

    tx = Wire.read();
    tx = (((uint16_t)tx) << 8) | Wire.read();
    ty = Wire.read();
    ty = (((uint16_t)ty) << 8) | Wire.read();
    tz = Wire.read();
    tz = (((uint16_t)tz) << 8) | Wire.read();
}

void HMC5883L::GetGaus(float& outX, float& outY, float& outZ){
    outX = tx * mgPerDigit + offsetX;
    outY = ty * mgPerDigit + offsetY;
    outZ = tz * mgPerDigit + offsetZ;
}

float HMC5883L::GetAngle(){
    
    float compass_x_scalled;
    float compass_y_scalled;
    float compass_z_scalled;

    GetGaus(compass_x_scalled, compass_y_scalled, compass_z_scalled);
    float bearing;


    if (compass_y_scalled>0){
        bearing = 90-atan(compass_x_scalled/compass_y_scalled)*57.3;
    }else if (compass_y_scalled<0){
        bearing = 270-atan(compass_x_scalled/compass_y_scalled)*57.3;
    }else if (compass_y_scalled==0 & compass_x_scalled<0){
        bearing = 180;
    }else{
        bearing = 0;
  }

  return bearing;
}


void HMC5883L::writeToRegister(uint8_t reg, uint8_t value){
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}