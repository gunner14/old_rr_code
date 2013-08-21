package com.dodoyo.invite.httpservice.entity;

import java.util.Map;

public class MailRequestParam {
    private int inviterId;
    private String appId;
    private String inviteeMail;
    private String timestamp;
    private String token;
    private String clientIp;
    private String action;
    private String templateId;
    private String fromer;
    private Map<String, String> statMap;

    public String getAppId() {
        return appId;
    }

    public void setAppId(String appId) {
        this.appId = appId;
    }

    public String getClientIp() {
        return clientIp;
    }

    public void setClientIp(String clientIp) {
        this.clientIp = clientIp;
    }

    public String getInviteeMail() {
        return inviteeMail;
    }

    public void setInviteeMail(String inviteeMail) {
        this.inviteeMail = inviteeMail;
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

    public String getTemplateId() {
        return templateId;
    }

    public void setTemplateId(String templateId) {
        this.templateId = templateId;
    }

    public String getFromer() {
        return fromer;
    }

    public void setFromer(String fromer) {
        this.fromer = fromer;
    }


    public Map<String, String> getStatMap() {
        return statMap;
    }

    public void setStatMap(Map<String, String> statMap) {
        this.statMap = statMap;
    }

    public String toString() {
        return "action=" + this.action + ";app_id=" + this.appId + ";invtee_mail=" +
                this.inviteeMail + ";inviter_id=" + this.inviterId + ";timestamp="
                + this.timestamp + ";token=" + this.token + ", fromer = " + this.fromer;
    }
}