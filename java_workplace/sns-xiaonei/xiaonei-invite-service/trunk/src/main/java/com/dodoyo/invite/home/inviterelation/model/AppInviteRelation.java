package com.dodoyo.invite.home.inviterelation.model;

import java.util.Date;


public class AppInviteRelation {

    public int id;
    public int inviterId;
    public int inviteeId;
    public int appId;
    public Date createTime;
    public long bak1;
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getInviterId() {
        return inviterId;
    }
    
    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }
    
    public int getInviteeId() {
        return inviteeId;
    }
    
    public void setInviteeId(int inviteeId) {
        this.inviteeId = inviteeId;
    }
    
    public int getAppId() {
        return appId;
    }
    
    public void setAppId(int appId) {
        this.appId = appId;
    }
    
    public Date getCreateTime() {
        return createTime;
    }
    
    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }
    
    public long getBak1() {
        return bak1;
    }
    
    public void setBak1(long bak1) {
        this.bak1 = bak1;
    }
    
    public String toString() {
        return "id=" + this.id + " inviteId=" + this.inviterId + " inviteeId=" + this.inviteeId + " appId=" + this.appId + " createTime=" + this.createTime + " bak1=" + this.bak1;  
    }
    
}
