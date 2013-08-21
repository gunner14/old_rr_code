package com.dodoyo.invite.httpservice.entity;

public class LinkRequestParam {
    private int inviterId;
    private String appId;
    private String groupName;
    private String baseLink;
    private String timestamp;
    private String token;
    private String clientIp;
    private String action;
    private String ss;
    private String rt;

    public String getAppId() {
        return appId;
    }

    public void setAppId(String appId) {
        this.appId = appId;
    }

    public String getBaseLink() {
        return baseLink;
    }

    public void setBaseLink(String baseLink) {
        this.baseLink = baseLink;
    }

    public String getClientIp() {
        return clientIp;
    }

    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public int getInviterId() {
        return inviterId;
    }

    public void setInviterId(int inviterId) {
        this.inviterId = inviterId;
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

    public String getAction() {
        return action;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public String getRt() {
        return rt;
    }

    public void setRt(String rt) {
        this.rt = rt;
    }

    public String getSs() {
        return ss;
    }

    public void setSs(String ss) {
        this.ss = ss;
    }

    public String toString() {
        return "action=" + this.action + ";app_id=" + this.appId + ";baselink=" + this.baseLink +
                ";group_name=" + this.groupName + ";inviter_id=" + this.inviterId + ";timestamp=" +
                this.timestamp + ";token=" + this.token;
    }

}