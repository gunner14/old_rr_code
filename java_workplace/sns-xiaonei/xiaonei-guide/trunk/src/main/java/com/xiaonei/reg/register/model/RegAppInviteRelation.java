package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 22, 2010 1:39:14 PM
 * 类说明
 */
public class RegAppInviteRelation implements Serializable {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -3731406820685638911L;
    // 主键
    private int id;
    // 新注册人的id即被邀请人的id
    private int inviteeId;
    // 邀请人的id
    private int inviterId;
    // 时间
    private Date crateTime;
    // 如果是app邀请，则该字段对应改app的id
    private int appId;
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getInviteeId() {
        return inviteeId;
    }
    
    public void setInviteeId(int inviteeId) {
        this.inviteeId = inviteeId;
    }
    
    public int getInviterId() {
        return inviterId;
    }
    
    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }
    
    public Date getCrateTime() {
        return crateTime;
    }
    
    public void setCrateTime(Date crateTime) {
        this.crateTime = crateTime;
    }
    
    public int getAppId() {
        return appId;
    }
    
    public void setAppId(int appId) {
        this.appId = appId;
    }
    
}
