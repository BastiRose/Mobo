#pragma once
#include "Component.h"
#include "Tasks.h"

#define MAX_TASKS 5

class TaskManager : public Component
{
    private:
        uint8_t taskCount = 0;
        Task* tasks[MAX_TASKS];
        Task* defaultTask;

        void popCurrentTask();
        
    public:
        void Setup(Task& defaultTask);
        void Update(uint32_t now);
        void AddTask(Task& task);
        void CancleAllTasks();
        void CancleCurrentTasks();
        bool IsCurrentTask(task_type_t type);
        task_type_t GetCurrentTaskType();
        void StartCurrentTask();
};

