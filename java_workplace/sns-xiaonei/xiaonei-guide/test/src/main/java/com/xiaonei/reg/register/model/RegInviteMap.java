package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

public class RegInviteMap implements Serializable {

    private static final long serialVersionUID = 132891811313664578L;

    // 主键
    private int id;
    // 新注册人的id即被邀请人的id
    private int ownerId;
    // 邀请人的id
    private int inviterId;
    // 时间
    private Date time;
    // 如果是app邀请，则该字段对应改app的id
    private int appId;
    // 邀请人邀请时使用的group id
    private String inviterGroupName;

    public String getInviterGroupName() {
        return inviterGroupName;
    }

    public void setInviterGroupName(String inviterGroupName) {
        this.inviterGroupName = inviterGroupName;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getOwnerId() {
        return ownerId;
    }

    public void setOwnerId(int ownerId) {
        this.ownerId = ownerId;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public int getAppId() {
        return appId;
    }

    public void setAppId(int appId) {
        this.appId = appId;
    }
}
