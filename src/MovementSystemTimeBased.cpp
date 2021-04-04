#include "MovementSystemTimeBased.h"
#include "Motor.h"

void MovementSystemTimeBased::Setup(Motor& motorLeft, Motor& motorRight, unsigned int milliDegreePerSec, unsigned int millimeterPerSec){
        this->motorLeft = &motorLeft;
        this->motorRight = &motorRight;
        this->milliDegreePerSec = milliDegreePerSec;
        this->millimeterPerSec = millimeterPerSec;
        this->intervallDegreeUpdate = max(1, (1000.0 / milliDegreePerSec) * 10);
        this->intervallDistanceUpdate = max(1, (1000.0 / millimeterPerSec) * 10);

        cIntervallDegreeUpdate = intervallDegreeUpdate;
        cIntervallDistanceUpdate = intervallDistanceUpdate;

        Serial.print("Movement System: intervallDegreeUpdate: ");
        Serial.println(this->intervallDegreeUpdate);
        Serial.print("Movement System: intervallDistanceUpdate: ");
        Serial.println(this->intervallDistanceUpdate);
}

void MovementSystemTimeBased::MoveForward(int speed, int distance, unsigned int smoothness){
        this->distance = 0;
        this->angle = 0;

        this->targetSpeed = speed;
        this->targetDistance = distance;
        this->targetAngle = 0;

        this->smoothnes = smoothnes;

        cIntervallDistanceUpdate = (intervallDistanceUpdate / 255.0) * (255 + (255 - speed));

        Serial.println("Movement System: Move Forward");
        Serial.print("Distance: ");
        Serial.println(distance);
        Serial.print("Speed: ");
        Serial.println(speed);
        Serial.print("Estimated Time: ");
        Serial.println((distance / 10) * cIntervallDistanceUpdate);

        state = 1;
}

void MovementSystemTimeBased::MoveBack(int speed, int distance, unsigned int smoothness){
        this->distance = 0;
        this->angle = 0;

        this->targetSpeed = speed;
        this->targetDistance = -distance;
        this->targetAngle = 0;

        cIntervallDistanceUpdate = (intervallDistanceUpdate / 255.0) * (255 + (255 - speed));

        this->smoothnes = smoothnes;

        Serial.println("Movement System: Backwards");
        Serial.print("Distance: ");
        Serial.println(distance);
        Serial.print("Speed: ");
        Serial.println(speed);
        Serial.print("Estimated Time: ");
        Serial.println((distance / 10) * cIntervallDistanceUpdate);
        state = 2;

}

void MovementSystemTimeBased::TurnOnPoint(int speed, int targetAngle, unsigned int smoothness){
        this->distance = 0;
        this->angle = 0;

        this->targetSpeed = speed;
        this->targetDistance = 0;
        this->targetAngle = targetAngle * 10;

        this->smoothnes = smoothnes;

        cIntervallDegreeUpdate = ((((intervallDegreeUpdate) * 100) / 255.0) * (255 + (255 - speed)) / 100 / 2);

        Serial.println("Movement System: Turn on Point");
        Serial.print("Angle: ");
        Serial.println(targetAngle);
        Serial.print("Speed: ");
        Serial.println(speed);
        Serial.print("Estimated Time: ");
        Serial.println((targetAngle / 10) * cIntervallDegreeUpdate);

        state = 3;
}

void MovementSystemTimeBased::MoveCurve(int speed, int degreePerSec, int distance, unsigned int smoothness){
        this->distance = 0;
        this->angle = 0;

        this->targetSpeed = speed;
        this->targetDistance = distance;
        this->targetAngle = degreePerSec;
 
        this->smoothnes = smoothnes;

        if(targetDistance != 0){
                this->timeToReachTargetDistance = ((distance / ((millimeterPerSec / 255.0) * speed))) * 1000;
                cIntervallDistanceUpdate = (intervallDistanceUpdate / 255.0) * (255 + (255 - speed));
        }

        this->motorSpeedForSlowerMotor = 255 - abs((float)(255.0 / (milliDegreePerSec)) * (degreePerSec * 10));
        cIntervallDegreeUpdate = 1000 / ((float)(milliDegreePerSec / 255.0) * motorSpeedForSlowerMotor);

        Serial.println("Movement System: Curve");
        Serial.print("Angle/s: ");
        Serial.println(targetAngle);
        Serial.print("Distance: ");
        Serial.println(targetDistance);
        Serial.print("Speed: ");
        Serial.println(speed);
        Serial.print("Speed slow: ");
        Serial.println(motorSpeedForSlowerMotor);
        Serial.print("Estimated Time: ");
        Serial.println((distance / 10) * cIntervallDistanceUpdate);

        state = 4;
}

void MovementSystemTimeBased::Stop(unsigned int smoothness){
        this->distance = 0;
        this->angle = 0;
        this->targetSpeed = 0;
        this->targetDistance = 0;
        this->targetAngle = 0;

        motorRight->ChangeSpeed(0);
        motorLeft->ChangeSpeed(0);
        state = 0;
}

void MovementSystemTimeBased::Brake(){
        this->distance = 0;
        this->angle = 0;
        this->targetSpeed = 0;
        this->targetDistance = 0;
        this->targetAngle = 0;

        motorRight->Brake();
        motorLeft->Brake();
        state = 0;
}

void MovementSystemTimeBased::Update(){

        motorLeft->Update();
        motorRight->Update();

        switch (state)
        {
                case 0:
                        if(motorRight->GetTargetSpeed() != 0 || motorLeft->GetTargetSpeed() != 0){
                                motorRight->ChangeSpeed(0);
                                motorLeft->ChangeSpeed(0);
                                Serial.println("Movement System: Slow Down");
                        } else {
                                if(motorRight->GetSpeed() == 0 && motorLeft->GetSpeed() == 0 && (!motorLeft->IsBraking() || !motorRight->IsBraking())){
                                        motorRight->Brake();
                                        motorLeft->Brake();
                                        Serial.println("Movement System: Brake (No Task)");
                                }
                        }
                        break;

                case 1:
                        doForward();
                        break;
                case 2:
                        doBackwards();
                        break;
                case 3:
                        doTrunOnPoint();
                        break;
                case 4:
                        doCurve();
                        break;
                default:
                        break;
        }

        if((state == 1 || state == 2 || state == 4) && targetDistance != 0){
                if(millis() - timerDistance >= cIntervallDistanceUpdate && targetDistance != 0){
                        timerDistance = millis();
                        if(distance < targetDistance)
                                distance += 10;
                        if(distance > targetDistance)
                                distance -= 10;
                }
        }

        if((state == 3 || state == 4) && targetAngle != 0){
                if(millis() - timerDegree >= cIntervallDegreeUpdate){       
                        timerDegree = millis();
                        if(angle < targetAngle)
                                angle += 10;
                        if(angle > targetAngle)
                                angle -= 10;          
                }
        }
}

byte MovementSystemTimeBased::GetProgress(){

}


void MovementSystemTimeBased::doForward(){

        if(distance != targetDistance || targetDistance == 0){
                if(motorLeft->GetTargetSpeed() != targetSpeed){
                        motorLeft->ChangeSpeed(targetSpeed);
                }
                if(motorRight->GetTargetSpeed() != targetSpeed){
                        motorRight->ChangeSpeed(targetSpeed);
                }
        } else {
                state = 0;
        }
}

void MovementSystemTimeBased::doBackwards(){

        if(distance != targetDistance || targetDistance == 0){
                if(motorLeft->GetTargetSpeed() != -targetSpeed){
                        motorLeft->ChangeSpeed(-targetSpeed);
                }
                if(motorRight->GetTargetSpeed() != -targetSpeed){
                        motorRight->ChangeSpeed(-targetSpeed);
                }
        } else {
                state = 0;
        }
}

void MovementSystemTimeBased::doTrunOnPoint(){

        if(angle != targetAngle || targetAngle == 0){   

                if(targetAngle > 0){
                        if(motorLeft->GetTargetSpeed() != -targetSpeed){
                                motorLeft->ChangeSpeed(-targetSpeed);
                        }
                        if(motorRight->GetTargetSpeed() != targetSpeed){
                                motorRight->ChangeSpeed(targetSpeed);
                        }
                }

                if(targetAngle < 0){
                        if(motorLeft->GetTargetSpeed() != targetSpeed){
                                motorLeft->ChangeSpeed(targetSpeed);
                        }
                        if(motorRight->GetTargetSpeed() != -targetSpeed){
                                motorRight->ChangeSpeed(-targetSpeed);
                        }
                } 
        } else {
                state = 0;
        }
}

void MovementSystemTimeBased::doCurve(){
        if((distance != targetDistance || targetDistance == 0)){

                if(targetAngle > 0){
                        if(motorLeft->GetTargetSpeed() != motorSpeedForSlowerMotor){
                                motorLeft->ChangeSpeed( motorSpeedForSlowerMotor);
                        }
                        if(motorRight->GetTargetSpeed() != targetSpeed){
                                motorRight->ChangeSpeed(targetSpeed);
                        }
                }

                if(targetAngle < 0){
                        if(motorLeft->GetTargetSpeed() != targetSpeed){
                                motorLeft->ChangeSpeed(targetSpeed);
                        }
                        if(motorRight->GetTargetSpeed() != motorSpeedForSlowerMotor){
                                motorRight->ChangeSpeed( motorSpeedForSlowerMotor);
                        }
                }
        } else {
                state = 0;
        } 
}