package com.xiaonei.xnmonitor.dto;

import java.util.Date;

public class ErrInfo {

    private long id;

    private String happenTime;

    private String errType;

    private String UIP;

    private String errServer;

    private String preDomain;

    private String preURL;

    private String curDomain;

    private String curURL;

    private long counter;

    private String preFulUrl;

    private String curFulUrl;

    private Date happenTimeDate;

    private int total;//在Http404500中画图用到的在时间区间里面出现的404或者500的次数

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getHappenTime() {
        return happenTime;
    }

    public void setHappenTime(String happenTime) {
        this.happenTime = happenTime;
    }

    public String getErrType() {
        return errType;
    }

    public void setErrType(String errType) {
        this.errType = errType;
    }

    public String getUIP() {
        return UIP;
    }

    public void setUIP(String uIP) {
        UIP = uIP;
    }

    public String getErrServer() {
        return errServer;
    }

    public void setErrServer(String errServer) {
        this.errServer = errServer;
    }

    public String getPreDomain() {
        return preDomain;
    }

    public void setPreDomain(String preDomain) {
        this.preDomain = preDomain;
    }

    public String getPreURL() {
        return preURL;
    }

    public void setPreURL(String preURL) {
        this.preURL = preURL;
    }

    public String getCurDomain() {
        return curDomain;
    }

    public void setCurDomain(String curDomain) {
        this.curDomain = curDomain;
    }

    public String getCurURL() {
        return curURL;
    }

    public void setCurURL(String curURL) {
        this.curURL = curURL;
    }

    public long getCounter() {
        return counter;
    }

    public void setCounter(long counter) {
        this.counter = counter;
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

    public Date getHappenTimeDate() {
        return happenTimeDate;
    }

    public void setHappenTimeDate(Date happenTimeDate) {
        this.happenTimeDate = happenTimeDate;
    }

    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }

}
