#include "MovementSystem.h"

void MovementSystem::popAction(){
    if(actionCount <= 0)
        return;

    Serial.print("Pop Movement Action ");
    Serial.print(actions[0]->GetID());

    actions[0]->Deactivate();
    for(int i = 1; i < maxActions - 1; i++){
        actions[i - 1] = actions[i];
    }

    actionCount--;

    if(actionCount > 0)
        actions[0]->Activate();
    

    Serial.print(": ");
    Serial.print(actionCount);
    Serial.print("/");
    Serial.println(maxActions);
}

void MovementSystem::Setup(MovementAction& idle, Motor& motorLeft, Motor& motorRight){
    this->idle = &idle;

    this->idle->Init(&motorLeft, &motorRight);

    this->motorLeft = &motorLeft;
    this->motorRight = &motorRight;
}

void MovementSystem::AddAction(MovementAction& action){
    if(actionCount >= maxActions){
        Serial.println("ERROR: To many Movement Actions. Skip!");
        return;
    }

    actions[actionCount] = &action;

    actions[actionCount]->Init(motorLeft, motorRight);

    if(actionCount == 0){
        actions[0]->Activate();  
    }

    Serial.print("Added Movement Action ");
    Serial.print(actions[actionCount]->GetID());
    Serial.print(": ");
    Serial.print(actionCount + 1);
    Serial.print("/");
    Serial.println(maxActions);

    actionCount++;
}

void MovementSystem::CancleCurrentAction(){
    popAction();
}

void MovementSystem::CancleAllActions(){
    if(actionCount > 0)
        actions[0]->Deactivate();

    actionCount = 0;
}

void MovementSystem::Update(){
    if(actionCount > 0){

        actions[0]->Execute();

        if(actions[0]->IsDone()){
            popAction();
        }

    } else {
        idle->Execute();
    }
}

uint8_t MovementSystem::GetProgress(){

}

bool MovementSystem::HasActionsLeft(){
    return actionCount > 0;
}

unsigned int MovementSystem::GetCurrentActionID(){
    if(actionCount > 0)
        return actions[0]->GetID();

        return idle->GetID();
}

bool MovementSystem::IsCurrentAction(MovementAction& action){
    if(actionCount > 0)
        return *actions[0] == action;

    return false;
}

bool MovementSystem::ContainsAction(MovementAction& action){
      for(int i = 0; i < actionCount; i++){
          if(*actions[i] == action)
            return true;
      }

      return false;
}