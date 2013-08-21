package com.renren.sns.guide.model;

public class GuideNewTaskView {

    private int taskId;

    private String taskName;

    private int praReward;

    private int rewardScore;

    private String rewardName;

    public GuideNewTaskView(int taskId, String taskName, int praReward, int rewardScore,
            String rewardName) {
        super();
        this.taskId = taskId;
        this.taskName = taskName;
        this.praReward = praReward;
        this.rewardScore = rewardScore;
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

    public String getRewardName() {
        return rewardName;
    }

    public void setRewardName(String rewardName) {
        this.rewardName = rewardName;
    }

}
