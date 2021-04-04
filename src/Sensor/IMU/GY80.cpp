#include "GY80.h"

    void GY80::Setup(){
        gyro.Setup(L3G4200D_SCALE_250DPS, L3G4200D_DATARATE_100HZ_25, 2);
        acc.Setup(ADXL345_RANGE_2G, ADXL345_DATARATE_100HZ);

        acc.Calibrate(100);
        gyro.Calibrate(100);

        timer1 = micros();
        timer2 = micros();
    }

    void GY80::Update(){
        if(micros() - timer1 >= 10000) {
            timer1 = micros();
            gyro.UpdateValues();
            acc.UpdateValues();

            if(gyro.HasNewValues() && acc.HasNewValues()){

                dt = (float)(micros() - timer2) / 1000000L;
                timer2 = micros();

                gyro.GetDPSXYZ(gx,gy,gz);
                acc.GetGXYZ(ax,ay,az);
                acc.GetAngle(ar, ap);

                gx *= dt; 
                gy *= dt; 
                gz *= dt; 

                aLength = sqrt(ax*ax + ay*ay + az*az);

                if(aLength > 0){
                    dotProductAG = (gx * (ax / aLength)  + gy * (ay / aLength)  + gz * (az / aLength)) ;  
                    yaw += dotProductAG;
                    headingYaw += dotProductAG;
                }

                roll = 0.95 * (roll + gx)  + 0.05 * ar;
                pitch = 0.95 * (pitch + gy)  - 0.05 * ap;
            }
        }
    }

    void GY80::SetHeading(){
        headingYaw = 0;
    }

    float GY80::GetHeadingError(){
        return headingYaw;
    }

    float GY80::GetRoll(){
        return roll;
    }

    float GY80::GetPitch(){
        return pitch;
    }

    float GY80::GetYaw(){
        return yaw;
    }