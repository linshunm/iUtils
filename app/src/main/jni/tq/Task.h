//
// Created by 林舜铭 on 2018/3/30.
//

#ifndef IUTILS_TASK_H
#define IUTILS_TASK_H

#include <string>

class Task {
public:
    const static int TASK_MAX_PRIORITY = 9;
    const static int TASK_MIN_PRIORITY = 0;
public:
    Task(std::string _id);
    ~Task();
    Task(const Task &_task);
    std::string getTqId();
    std::string getTaskId();
    void setTimeout(long _timeout);
    long getTimeout();
    bool isProcessing();
    void setProcessing(bool _isDoing);
    void doTask();
    int getPriority();
    void setPriority(int _priority);
    static void* run(void *args);

private:
    std::string taskId;
    std::string tqId;
    int priority;
    long timeout;//默认两分钟
    long startTaskTime;
    bool processing;
};


#endif //IUTILS_TASK_H
