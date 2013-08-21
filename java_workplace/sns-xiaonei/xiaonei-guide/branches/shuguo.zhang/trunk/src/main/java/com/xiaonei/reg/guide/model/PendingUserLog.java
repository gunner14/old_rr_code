package com.xiaonei.reg.guide.model;

import java.util.Date;

public class PendingUserLog {

    private int id;
    private int usr;
    private Date time;
    private int admin;
    private String adminName;
    private Date optime;
    private int op;

    public int getAdmin() {
        return admin;
    }

    public void setAdmin(int admin) {
        this.admin = admin;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getOp() {
        return op;
    }

    public void setOp(int op) {
        this.op = op;
    }

    public Date getOptime() {
        return optime;
    }

    public void setOptime(Date optime) {
        this.optime = optime;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public int getUsr() {
        return usr;
    }

    public void setUsr(int usr) {
        this.usr = usr;
    }

    public String getAdminName() {
        return adminName;
    }

    public void setAdminName(String adminName) {
        this.adminName = adminName;
    }
}
