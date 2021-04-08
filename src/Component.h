#pragma once
#include <Arduino.h>

class Component
{
protected:
    bool enabled = true;
    uint8_t priority = 0;

public:
    virtual void Update(uint32_t now) = 0;

    uint8_t GetPriority(){
        return priority;
    }

    void SetPriority(uint8_t priority){
        this->priority = priority;
    }
    
   bool IsEnabled(){
       return enabled;
   }

   void SetEnabled(bool enabled){
       this->enabled = enabled;
   }
};
