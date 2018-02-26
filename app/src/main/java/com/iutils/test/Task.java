package com.iutils.test;

public class Task {
    public void setTaskId(String taskId) {
        this.taskId = taskId;
    }

    protected String taskId;//任务Id
    private int priority = 0;//任务优先级,数值越大，优先级越高,且数值在0~9以内
    private boolean isDoing = false;

    public void setPriority(int priority) {
        this.priority = priority;
    }


    public boolean isDoing() {
        return isDoing;
    }

    public void setDoing(boolean doing) {
        isDoing = doing;
    }

    public int getPriority() {
        return priority;
    }

    @Override
    public String toString() {
        return "Task{" +
                "taskId='" + taskId + '\'' +
                ", priority=" + priority +
                ", isDoing=" + isDoing +
                '}';
    }
}

