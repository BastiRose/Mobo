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

uint8_t minuteCounter = 0;
unsigned long timeActive = 0;

void setup(){
    Serial.begin(115200);
    Serial2.begin(115200);
    SetupRobot(robot);
    timer = millis();

    delay(100);
}

void loop(){

    if(robot.Now - timer >= 1000){
        minuteCounter++;
        
        Serial.print("Loops/s: ");
        Serial.println(counter);
        lps = counter;
        counter = 0;
        timer = millis();
       if(espReadyForUpdate)
         UpdateESP();
      else
        espReadyForUpdate = true;
    }

    if(minuteCounter >= 60){
      timeActive++;
      minuteCounter = 0;
    }

    CheckSerial();

    robot.Loop();
    counter++;
}

void UpdateESP(){
  espReadyForUpdate = false;
  Serial2.flush();
  Serial2.print('U');
  Serial2.print('|');
  Serial2.print('M');
  Serial2.print('|');
  Serial2.print(MotorLeft.GetSpeed());
  Serial2.print('|');
  Serial2.print(CurrentSensorMotorLeft.GetCurrent() / 100);
  Serial2.print('|');
  Serial2.print(MotorLeftSensing.Ckeck());
  Serial2.print('|');
  Serial2.print(MotorRight.GetSpeed());
  Serial2.print('|');
  Serial2.print(CurrentSensorMotorRight.GetCurrent() / 100);
  Serial2.print('|');
  Serial2.print(MotorRightSensing.Ckeck());
  Serial2.print('|');
  Serial2.print(MotorMower.GetSpeed());
  Serial2.print('#');

  Serial2.print('S');
  Serial2.print('|');
  Serial2.print(robot.Tasks->GetCurrentTaskType());
  Serial2.print('|');
  Serial2.print(robot.Battery->GetMillivoltage());
  Serial2.print('|');
  Serial2.print(robot.Battery->IsCharging()); //dock
  Serial2.print('|');
  Serial2.print(EmergencyButton.IsActive()); //switch
  Serial2.print('|');
  Serial2.print(lps);
  Serial2.print('|');
  unsigned long x = (robot.Tasks->TimeInCurrentTask() / 1000) / 60;
  Serial2.write((uint8_t *) &timeActive, sizeof(timeActive));
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
    Serial2.print(UsSensor.GetDistance());
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
  Serial2.print(" Near: ");
  Serial2.print(robot.BoundarySensor->IsNear());
  Serial2.print(" In Out: ");
  Serial2.print(robot.BoundarySensor->LastTimePassedBoundary() / 1000);

  Serial2.print(" | IMU: Roll: ");
  Serial2.print((int)robot.IMU->GetRoll());
  Serial2.print(" Pitch ");
  Serial2.print((int)robot.IMU->GetPitch());
  Serial2.print(" Yaw ");
  Serial2.print((int)robot.IMU->GetYaw());

  Serial2.print(" | Mower Dir: ");
  Serial2.print((int)robot.MowerMotor->GetDirection());

  Serial2.print(" | Battery: Need Charging: ");
  Serial2.print(robot.Battery->NeedCharging());
  Serial2.print(" | Battery: Low: ");
  Serial2.print(robot.Battery->IsLow());

  Serial2.print(" | Task: ");
  Serial2.print((int)robot.Tasks->GetCurrentTaskType());




  Serial2.print('\0');
  Serial2.print('#');
  Serial2.print("\n");
}

TaskMow mowTask;
TaskPause pauseTask;
TaskStop stopTask;
TaskGoHome goHomeTask;

void CheckSerial(){
  if(Serial2.available()){
    char c = Serial2.read();

    if(c == '('){
        delay(5);
        char c = Serial2.read();

        if(c == 'g'){

            robot.Tasks->CancleAllTasks();
            robot.Tasks->AddTask(goHomeTask);
            Serial.println("COMMAND GO HOME");

        }

        if(c == 's'){
            if(robot.Tasks->IsCurrentTask(TaskPause::Type)){
              robot.Tasks->CancleAllTasks();
              robot.Tasks->AddTask(stopTask);
              Serial.println("COMMAND GO Stop");
            } else {
              robot.Tasks->CancleAllTasks();
              robot.Tasks->AddTask(pauseTask);
              Serial.println("COMMAND GO Pause");
            }

        }

        if(c == 'm'){
            mowTask.Setup(0, *robot.Battery);
            robot.Tasks->CancleAllTasks();
            robot.Tasks->AddTask(mowTask);
            if(robot.Battery->IsCharging())
              robot.Tasks->AddTask(goHomeTask);
            Serial.println("COMMAND GO MOW");
        }

      if(c == 'u'){
        espReadyForUpdate = true;
      }
    }
  }
}
