package com.dodoyo.invite.core.entity;

public class TemplateContent {
    protected String contentId;
    protected String appId;
    protected String vmTemplate;
    protected String title;
    protected String body;
    protected String sourceId;
    protected String disc;
    protected String rt;
    protected String link;
    protected String sender;

    public TemplateContent(TemplateContent tc) {
        this.contentId = tc.getContentId();
        this.appId = tc.getAppId();
        this.vmTemplate = tc.getVmTemplate();
        this.title = tc.getTitle();
        this.body = tc.getBody();
        this.sourceId = tc.getSourceId();
        this.disc = tc.getDisc();
        this.rt = tc.getRt();
        this.link = tc.getLink();
        this.sender = tc.getSender();
    }

    public TemplateContent() {
    }

    public String getAppId() {
        return appId;
    }

    public void setAppId(String appId) {
        this.appId = appId;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public String getContentId() {
        return contentId;
    }

    public void setContentId(String contentId) {
        this.contentId = contentId;
    }

    public String getDisc() {
        return disc;
    }

    public void setDisc(String disc) {
        this.disc = disc;
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }

    public String getRt() {
        return rt;
    }

    public void setRt(String rt) {
        this.rt = rt;
    }

    public String getSourceId() {
        return sourceId;
    }

    public void setSourceId(String sourceId) {
        this.sourceId = sourceId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getVmTemplate() {
        return vmTemplate;
    }

    public void setVmTemplate(String vmTemplate) {
        this.vmTemplate = vmTemplate;
    }

    public String getSender() {
        return sender;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }

    public String toString() {
        return "contentId:" + this.contentId + ", title = " + this.title + ", body = " + this.body + "" +
                ", sourceId = " + this.sourceId + ", vmTemplate = " + this.vmTemplate + ", rt = " + this.rt + "" +
                ", link = " + this.link + ", appId = " + this.appId + ", disc = " + this.disc + "sender = " + this.sender;
    }
}