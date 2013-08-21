package com.xiaonei.reg.usertrace.model;

import java.io.Serializable;
import java.util.Date;

public class RegVisitHis implements Serializable {

    private static final long serialVersionUID = 132891811313664578L;

    //主键
    private int id;
    //此页面或功能的名字
    private String funName;
    //cookie中的sessionId
    private String xnSessId;
    //IP
    private String ip;
    //登录着的Id
    private int userId;
    //时间
    private Date time;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getFunName() {
        return funName;
    }

    public void setFunName(String funName) {
        this.funName = funName;
    }

    public String getXnSessId() {
        return xnSessId;
    }

    public void setXnSessId(String xnSessId) {
        this.xnSessId = xnSessId;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

}
