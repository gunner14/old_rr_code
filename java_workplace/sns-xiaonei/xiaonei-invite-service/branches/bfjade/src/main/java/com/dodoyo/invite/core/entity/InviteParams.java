package com.dodoyo.invite.core.entity;

import java.io.Serializable;
import java.util.Map;

public class InviteParams implements Serializable {
    //invite basic
    private int inviterId;
    private String inviteeMail;
    private String templateId;
    private String retURL;
    private String appId;
    private String appToken;
    private String specSs;
    private String specRt;
    private int actionType;
    private String buddyGroup;
    private String code;
    private int pollingId;
    private String appHtml;
    private String customMessage;

    private String importerMail;

    private Map<String, String> statMap;
    //from template
    private TemplateContent templateContent;

    //for uuid
    private String uuid;

    public int getActionType() {
        return actionType;
    }

    public void setActionType(int actionType) {
        this.actionType = actionType;
    }

    public String getAppId() {
        return appId;
    }

    public void setAppId(String appId) {
        this.appId = appId;
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

    public String getRetURL() {
        return retURL;
    }

    public void setRetURL(String retURL) {
        this.retURL = retURL;
    }

    public String getTemplateId() {
        return templateId;
    }

    public void setTemplateId(String templateId) {
        this.templateId = templateId;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public TemplateContent getTemplateContent() {
        return templateContent;
    }

    public void setTemplateContent(TemplateContent templateContent) {
        this.templateContent = templateContent;
    }


    public String getAppToken() {
        return appToken;
    }

    public void setAppToken(String appToken) {
        this.appToken = appToken;
    }


    public String getBuddyGroup() {
        return buddyGroup;
    }

    public void setBuddyGroup(String buddyGroup) {
        this.buddyGroup = buddyGroup;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public int getPollingId() {
        return pollingId;
    }

    public void setPollingId(int pollingId) {
        this.pollingId = pollingId;
    }

    public String getAppHtml() {
        return appHtml;
    }

    public void setAppHtml(String appHtml) {
        this.appHtml = appHtml;
    }

    public String getCustomMessage() {
        return customMessage;
    }

    public void setCustomMessage(String customMessage) {
        this.customMessage = customMessage;
    }

    public String getSpecSs() {
        return specSs;
    }

    public void setSpecSs(String specSs) {
        this.specSs = specSs;
    }


    public String getSpecRt() {
        return specRt;
    }

    public void setSpecRt(String specRt) {
        this.specRt = specRt;
    }

    public String getImporterMail() {
        return importerMail;
    }

    public void setImporterMail(String importerMail) {
        this.importerMail = importerMail;
    }

    public Map<String, String> getStatMap() {
        return statMap;
    }

    public void setStatMap(Map<String, String> statMap) {
        this.statMap = statMap;
    }

    public String toString() {
        return "inviterId = " + this.inviterId + ", inviteeMail = " + this.inviteeMail + ", templateId = " + this.templateId + "" +
                ", uuid = " + this.uuid + ", templateContent = " + this.templateContent + ", actionType = " + this.actionType;
    }
}