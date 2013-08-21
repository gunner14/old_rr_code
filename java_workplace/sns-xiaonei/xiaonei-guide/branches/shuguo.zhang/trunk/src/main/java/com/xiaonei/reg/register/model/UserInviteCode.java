package com.xiaonei.reg.register.model;

import java.io.Serializable;

public class UserInviteCode implements Serializable {
    private static final long serialVersionUID = 7004288930369760212L;
    private int id;
    private String inviteCode;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getInviteCode() {
        return inviteCode;
    }

    public void setInviteCode(String inviteCode) {
        this.inviteCode = inviteCode;
    }
}