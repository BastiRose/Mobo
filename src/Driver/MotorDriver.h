#pragma once
#include <Arduino.h>
#include "../Component.h"

class MotorDriver : public Component{
    protected:
        bool brake = 0;
    public:
        
        virtual void Update(uint32_t now) = 0;
        virtual void ChangeSpeed(const int speed) = 0;

        virtual void Brake() = 0;
        virtual int GetSpeed() = 0;
        bool IsBraking(){
            return brake;
        }

        virtual void Enable(bool enable) = 0;


};



