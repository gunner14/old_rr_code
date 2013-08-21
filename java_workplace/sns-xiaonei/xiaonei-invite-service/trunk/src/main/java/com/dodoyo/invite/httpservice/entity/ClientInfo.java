package com.dodoyo.invite.httpservice.entity;

import java.sql.Timestamp;

public class ClientInfo {
    private int id;
    private String clientName;
    private String clientIp;
    private String clientKey;
    private Integer type;
    private Timestamp expireTime;
    private Timestamp createTime;
    private Integer bak1;
    private Integer bak2;
    private String bak3;
    private String bak4;
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public String getClientName() {
        return clientName;
    }
    
    public void setClientName(String clientName) {
        this.clientName = clientName;
    }
    
    public String getClientIp() {
        return clientIp;
    }
    
    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }
    
    public String getClientKey() {
        return clientKey;
    }
    
    public void setClientKey(String clientKey) {
        this.clientKey = clientKey;
    }
    
    public Integer getType() {
        return type;
    }
    
    public void setType(Integer type) {
        if (type == null) {
            this.type = 0;
        } else {
            this.type = type;
        }
    }
    
    public Timestamp getExpireTime() {
        return expireTime;
    }
    
    public void setExpireTime(Timestamp expireTime) {
        this.expireTime = expireTime;
    }
    
    public Timestamp getCreateTime() {
        return createTime;
    }
    
    public void setCreateTime(Timestamp createTime) {
        this.createTime = createTime;
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