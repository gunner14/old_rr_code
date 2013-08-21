package com.xiaonei.reg.guide.model;

import java.io.Serializable;

public class UserBootStatus implements Serializable {

    private static final long serialVersionUID = 7561380886489113879L;

    private int id; // user id
    private int inviteStatus;
    private int passedAudit;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getInviteStatus() {
        return inviteStatus;
    }

    public void setInviteStatus(int inviteStatus) {
        this.inviteStatus = inviteStatus;
    }

    public int getPassedAudit() {
        return passedAudit;
    }

    public void setPassedAudit(int passedAudit) {
        this.passedAudit = passedAudit;
    }
}
