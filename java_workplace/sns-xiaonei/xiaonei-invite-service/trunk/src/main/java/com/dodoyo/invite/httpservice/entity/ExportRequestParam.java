package com.dodoyo.invite.httpservice.entity;

import java.util.Map;

public class ExportRequestParam {
    private int inviterId;
    private String action;
    private String email;
    private boolean isMsn;
    private String password;
    private String appId;
    private String timestamp;
    private String token;
    private String httpCallback;
    private Map<String, Object> attMap;

    private String clientIp;

    private int listType;

    public String getAction() {
        return action;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getHttpCallback() {
        return httpCallback;
    }

    public void setHttpCallback(String httpCallback) {
        this.httpCallback = httpCallback;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
    }

    public boolean isMsn() {
        return isMsn;
    }

    public void setMsn(boolean isMsn) {
        this.isMsn = isMsn;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(String timestamp) {
        this.timestamp = timestamp;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getClientIp() {
        return clientIp;
    }

    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }

    public String getAppId() {
        return appId;
    }

    public void setAppId(String appId) {
        this.appId = appId;
    }

    public int getListType() {
        return listType;
    }

    public void setListType(int listType) {
        this.listType = listType;
    }

    public Map<String, Object> getAttMap() {
        return attMap;
    }

    public void setAttMap(Map<String, Object> attMap) {
        this.attMap = attMap;
    }

    public String toString() {
        return "action=" + this.action + ";app_id=" + this.appId + ";client_ip=" + this.clientIp
                + ";email=" + this.email + ";httpcallback=" +
                this.httpCallback + ";inviterId=" + this.inviterId + ";password=" + this.password +
                ";timestamp=" + this.timestamp + ";token=" +
                this.token;
    }


}