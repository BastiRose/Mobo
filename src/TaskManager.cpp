#include "TaskManager.h"

void TaskManager::popCurrentTask(){
    if(taskCount <= 0)
        return;

    Serial.print("Pop Task with type ");
    Serial.print(tasks[0]->GetType());


    for(int i = 1; i < MAX_TASKS - 1; i++){
        tasks[i - 1] = tasks[i];
    }

    taskCount--;

    if(taskCount > 0)
        tasks[0]->Activate();

    Serial.print(": ");
    Serial.print(taskCount);
    Serial.print("/");
    Serial.println(MAX_TASKS);
}

void TaskManager::Update(uint32_t now){
     if(taskCount == 0)
        return;

    if (tasks[0]->Check()){
        popCurrentTask();
    }
}

void TaskManager::AddTask(Task& task){
    if(taskCount >= MAX_TASKS){
        Serial.println("!!! TOO MANY TASKS !!!");
        return;
    }

    tasks[taskCount] = &task;
    if(taskCount == 0)
        tasks[taskCount]->Activate();

    taskCount++;
}

void TaskManager::CancleAllTasks(){
    taskCount = 0;
}

void TaskManager::CancleCurrentTasks(){
    popCurrentTask();
}

bool TaskManager::IsCurrentTask(task_type_t type){
    if(taskCount == 0)
        return false;

    return tasks[0]->GetType() == type;
}

task_type_t TaskManager::GetCurrentTaskType(){
    if(taskCount == 0)
        return Task_None;

    return tasks[0]->GetType();
}

void TaskManager::StartCurrentTask(){
    if(taskCount != 0){
        tasks[0]->Start();
    }
}