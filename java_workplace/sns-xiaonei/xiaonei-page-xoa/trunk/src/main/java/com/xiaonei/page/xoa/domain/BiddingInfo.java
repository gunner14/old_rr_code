package com.xiaonei.page.xoa.domain;


public class BiddingInfo {
    private int bidingUp;
    private String sendFeed;
    private int targetWeight;
    private int budgetFund;
    public BiddingInfo(){
        
    }
    public BiddingInfo(int bidingUp, String sendFeed, int targetWeight, int budgetFund) {
        this.bidingUp = bidingUp;
        this.sendFeed = sendFeed;
        this.targetWeight = targetWeight;
        this.budgetFund = budgetFund;
    }

    
    public int getBidingUp() {
        return bidingUp;
    }

    
    public void setBidingUp(int bidingUp) {
        this.bidingUp = bidingUp;
    }

    
    public String getSendFeed() {
        return sendFeed;
    }

    
    public void setSendFeed(String sendFeed) {
        this.sendFeed = sendFeed;
    }

    
    public int getTargetWeight() {
        return targetWeight;
    }

    
    public void setTargetWeight(int targetWeight) {
        this.targetWeight = targetWeight;
    }

    
    public int getBudgetFund() {
        return budgetFund;
    }

    
    public void setBudgetFund(int budgetFund) {
        this.budgetFund = budgetFund;
    }
    
    
    
    
}
