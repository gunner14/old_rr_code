package com.renren.sns.guide.model;

public class GuideNewTask {

    private int taskId;

    private String taskName;

    private int praReward;

    private int rewardScore;

    private String rewardName;
    
    private int taskFinished;
    
    public GuideNewTask() {
        super();
    }

    public GuideNewTask(int taskId, String taskName, int praReward, int rewardScore,
            String rewardName, int taskFinished) {
        super();
        this.taskId = taskId;
        this.taskName = taskName;
        this.praReward = praReward;
        this.rewardScore = rewardScore;
        this.rewardName = rewardName;
        this.taskFinished = taskFinished;
    }

    public int getTaskId() {
        return taskId;
    }

    public void setTaskId(int taskId) {
        this.taskId = taskId;
    }

    public String getTaskName() {
        return taskName;
    }

    public void setTaskName(String taskName) {
        this.taskName = taskName;
    }

    public int getPraReward() {
        return praReward;
    }

    public void setPraReward(int praReward) {
        this.praReward = praReward;
    }

    public int getRewardScore() {
        return rewardScore;
    }

    public void setRewardScore(int rewardScore) {
        this.rewardScore = rewardScore;
    }

    public int getTaskFinished() {
        return taskFinished;
    }

    public void setTaskFinished(int taskFinished) {
        this.taskFinished = taskFinished;
    }

    public String getRewardName() {
        return rewardName;
    }

    public void setRewardName(String rewardName) {
        this.rewardName = rewardName;
    }

}
