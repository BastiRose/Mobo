#pragma once
#include <Arduino.h>
#include "MovementAction.h"
#include "Component.h"

class MovementSystem: public Component{
    private:
        MovementAction* actions[10];
        uint8_t maxActions = 10;

        uint8_t actionCount = 0;

        MovementAction* idle;

        unsigned long timeEnterCurrentAction = 0;

        Motor* motorLeft;
        Motor* motorRight;

        void popAction();

        void activateCurrentAction();
        
    public:
        void Setup(MovementAction& idle, Motor& motorLeft, Motor& motorRight);
        void AddAction(MovementAction& action);
        void CancleCurrentAction();
        void CancleAllActions();
        void Update(uint32_t now);
        uint8_t GetProgress();
        bool HasActionsLeft();
        
        unsigned int GetCurrentActionID();

        unsigned int long GetTimeInCurrentAction();

        bool IsCurrentAction(MovementAction& action);
        bool ContainsAction(MovementAction& action);
};

