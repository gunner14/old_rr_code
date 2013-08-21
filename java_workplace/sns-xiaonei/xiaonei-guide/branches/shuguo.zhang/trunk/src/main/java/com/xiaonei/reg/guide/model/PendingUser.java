package com.xiaonei.reg.guide.model;

import java.io.Serializable;
import java.util.Date;

public class PendingUser implements Serializable {

    public static final int FLAG_DEFAULT = 1;
    public static final int FLAG_APPLY = 0;
    private static final long serialVersionUID = 4633947040586714992L;
    private int id;
    private Date time;
    private int user;
    private int stage;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public PendingUser(int usr) {
        super();
        this.user = usr;
        this.time = new Date();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public int getUser() {
        return user;
    }

    public void setUser(int user) {
        this.user = user;
    }
}
