#pragma once
#include <Arduino.h>
#include "MovementAction.h"

class MovementSystem{
    private:
        MovementAction* actions[10];
        uint8_t maxActions = 10;

        uint8_t actionCount = 0;

        MovementAction* idle;

        Motor* motorLeft;
        Motor* motorRight;

        void popAction();
        
    public:
        void Setup(MovementAction& idle, Motor& motorLeft, Motor& motorRight);
        void AddAction(MovementAction& action);
        void CancleCurrentAction();
        void CancleAllActions();

        void Update();
        uint8_t GetProgress();
        bool HasActionsLeft();
        
        unsigned int GetCurrentActionID();

        bool IsCurrentAction(MovementAction& action);
        bool ContainsAction(MovementAction& action);
};

