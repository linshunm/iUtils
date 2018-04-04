//
// Created by 林舜铭 on 2018/3/30.
//

#include "Task.h"
#include "LogUtils.h"

Task::Task(std::string _id) {
    LOGI("TASK created %s", _id.c_str());
    taskId = _id;
    long timeout = 2 * 60 * 1000;//默认两分钟
    long startTaskTime = 0L;
    bool isDoing = false;
}

Task::~Task() {
    LOGI("TASK destroy %s", taskId.c_str());
}

std::string Task::getTqId()
{
    return tqId;
}
std::string Task::getTaskId()
{
    return taskId;
}
long Task::getTimeout()
{
    return timeout;
}
void Task::setTimeout(long _timeout)
{
    timeout = _timeout;
}


bool Task::isProcessing()
{
    return processing;
}
void Task::setProcessing(bool _isDoing)
{
    processing = _isDoing;
}
void Task::doTask()
{
    startTaskTime = 0L;
}


int Task::getPriority()
{
    return priority;
}

void Task::setPriority(int _priority)
{
    if(priority > TASK_MAX_PRIORITY)
    {
        priority = TASK_MAX_PRIORITY;
    }
    else if(priority < TASK_MIN_PRIORITY)
    {
        priority = TASK_MIN_PRIORITY;
    }
    priority = _priority;
}