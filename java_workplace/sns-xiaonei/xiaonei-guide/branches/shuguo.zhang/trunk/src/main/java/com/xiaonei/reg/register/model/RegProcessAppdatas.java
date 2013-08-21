package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

public class RegProcessAppdatas implements Serializable {

    private static final long serialVersionUID = 132891811313664578L;

    //主键
    private int id;
    //应用Id
    private int appId;
    //邀请者id
    private int inviterId;
    //被邀请者id
    private int inviteeId;
    //处理时间
    private int processingTime;
    //创建时间
    private Date createTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAppId() {
        return appId;
    }

    public void setAppId(int appId) {
        this.appId = appId;
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

    public int getProcessingTime() {
        return processingTime;
    }

    public void setProcessingTime(int processingTime) {
        this.processingTime = processingTime;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

}
