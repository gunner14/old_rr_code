package com.xiaonei.tools.monitor.entity;

public class HttpErrEntity {

    private String happernTime;

    private String errorType;

    private String UIP;

    private String errorServer;

    private String preDomain;

    private String curDomain;

    private String preUrl;

    private String curUrl;

    private String preFulUrl;

    private String curFulUrl;

    private String keyWord;//标识 renren ,kaixin ,xiaonei.

    public String getHappernTime() {
        return happernTime;
    }

    public void setHappernTime(String happernTime) {
        this.happernTime = happernTime;
    }

    public String getErrorType() {
        return errorType;
    }

    public void setErrorType(String errorType) {
        this.errorType = errorType;
    }

    public String getUIP() {
        return UIP;
    }

    public void setUIP(String uIP) {
        UIP = uIP;
    }

    public String getErrorServer() {
        return errorServer;
    }

    public void setErrorServer(String errorServer) {
        this.errorServer = errorServer;
    }

    public String getPreDomain() {
        return preDomain;
    }

    public void setPreDomain(String preDomain) {
        this.preDomain = preDomain;
    }

    public String getCurDomain() {
        return curDomain;
    }

    public void setCurDomain(String curDomain) {
        this.curDomain = curDomain;
    }

    public String getPreUrl() {
        return preUrl;
    }

    public void setPreUrl(String preUrl) {
        this.preUrl = preUrl;
    }

    public String getCurUrl() {
        return curUrl;
    }

    public void setCurUrl(String curUrl) {
        this.curUrl = curUrl;
    }

    public String getPreFulUrl() {
        return preFulUrl;
    }

    public void setPreFulUrl(String preFulUrl) {
        this.preFulUrl = preFulUrl;
    }

    public String getCurFulUrl() {
        return curFulUrl;
    }

    public void setCurFulUrl(String curFulUrl) {
        this.curFulUrl = curFulUrl;
    }

    public String getKeyWord() {
        return keyWord;
    }

    public void setKeyWord(String keyWord) {
        this.keyWord = keyWord;
    }

    //    String url;
    //    String referer;
    //    String 

}
