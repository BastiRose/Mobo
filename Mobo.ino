#include "src/Robot.h"
#include "src/SetupMoboOne.h"
#include <Ramp.h>
#include <HighLowPass.h>

Robot robot;

unsigned long timer;
unsigned long counter;
unsigned int lps = 0;
int state = 0;
bool espReadyForUpdate;

void setup(){
    Serial.begin(115200);
    Serial2.begin(115200);
    SetupRobot(robot);
    timer = millis();

    delay(100);
}

void loop(){

    if(millis() - timer >= 1000){
        Serial.print("Loops/s: ");
        Serial.println(counter);
        lps = counter;
        counter = 0;
        timer = millis();

    }

    if(espReadyForUpdate){
        UpdateESP();
        espReadyForUpdate = false;
    } else {
        CheckSerial();
    }

    robot.Loop();
    counter++;
}

void UpdateESP(){
  
  Serial2.print('U');
  Serial2.print('|');
  Serial2.print('M');
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('#');

  Serial2.print('S');
  Serial2.print('|');
  Serial2.print(0);
  Serial2.print('|');
  Serial2.print(robot.Battery->GetMillivoltage());
  Serial2.print('|');
  Serial2.print(0); //dock
  Serial2.print('|');
  Serial2.print(0); //switch
  Serial2.print('|');
  Serial2.print(lps);
  Serial2.print('|');
  unsigned long x = 0;
  Serial2.write((uint8_t *) &x, sizeof(x));
  Serial2.print('|');
  Serial2.write((uint8_t *) &x, sizeof(x));
  Serial2.print('#');

  Serial2.print('I');
  Serial2.print('|');
  Serial2.print((int)robot.IMU->GetHeadingError());
  Serial2.print('#');

  Serial2.print('R');
  Serial2.print('|');
  if(robot.ObjectDetection->HasObjectDetected())
    Serial2.print(robot.ObjectDetection->GetClosestObject().Distance);
  else
    Serial2.print(0);
  Serial2.print('#');

  Serial2.print('W');
  Serial2.print('|');
  Serial2.print(robot.BoundarySensor->IsActive());
  Serial2.print('|');
  Serial2.print(robot.BoundarySensor->IsInside());
  Serial2.print('|');
  Serial2.print(robot.BoundarySensor->BoundaryError());
  Serial2.print('#');

  Serial2.print('T');
  Serial2.print("In state ");
  Serial2.print(robot.MainStateMachine.currentState->Name);
  Serial2.print(" for ");
  Serial2.print(robot.MainStateMachine.currentState->timeInState / 1000);
  Serial2.print(" sec ");
  Serial2.print(" | Wire Max Mag: ");
  Serial2.print(robot.BoundarySensor->BoundaryErrorMax());
  Serial2.print(" Near ");
  Serial2.print(robot.BoundarySensor->IsNear());
  Serial2.print(" Mower: ");
  Serial2.print(0);
  Serial2.print(" mA ");

  Serial2.print(" IMU: Roll: ");
  Serial2.print((int)robot.IMU->GetRoll());
  Serial2.print(" Pitch ");
  Serial2.print((int)robot.IMU->GetPitch());
  Serial2.print(" Yaw ");
  Serial2.print((int)robot.IMU->GetYaw());



  Serial2.print('\0');
  Serial2.print('#');
  Serial2.print("\n");
}


void CheckSerial(){
  if(Serial2.available()){
    char c = Serial2.read();

    if(c == '('){
        char c = Serial2.read();


      if(c == 'u'){
        espReadyForUpdate = true;
      }
    }
  }
}
