package com.dodoyo.invite.httpservice.entity;

import java.sql.Timestamp;

public class Token {
    private int id;
    private String token;
    private String clientIp;
    private String action;
    private String params;
    private Timestamp createTime;
    private Integer bak1;
    private Integer bak2;
    private String bak3;
    private String bak4;

    public String getAction() {
        return action;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public String getClientIp() {
        return clientIp;
    }

    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }

    public Timestamp getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Timestamp createTime) {
        this.createTime = createTime;
    }

    public String getParams() {
        return params;
    }

    public void setParams(String params) {
        this.params = params;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
    
    public Integer getBak1() {
        return bak1;
    }
    
    public void setBak1(Integer bak1) {
        if (bak1 == null) {
            this.bak1 = 0;
        } else {
            this.bak1 = bak1;
        }
    }
    
    public Integer getBak2() {
        return bak2;
    }
    
    public void setBak2(Integer bak2) {
        if (bak2 == null) {
            this.bak2 = 0;
        } else {
            this.bak2 = bak2;
        }
    }

    public String getBak3() {
        return bak3;
    }

    public void setBak3(String bak3) {
        this.bak3 = bak3;
    }
    
    public String getBak4() {
        return bak4;
    }
    
    public void setBak4(String bak4) {
        this.bak4 = bak4;
    }

}