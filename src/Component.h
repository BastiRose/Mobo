#pragma once

class Component
{
protected:
    bool enabled = true;
public:
    virtual void Update(){
        if(!enabled) return;
    }
    
   bool IsEnabled(){
       return enabled;
   }

   void SetEnabled(bool enabled){
       this->enabled = enabled;
   }
};
