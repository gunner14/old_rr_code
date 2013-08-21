package com.xiaonei.xnmonitor.dto;

public class ErrorDetails extends BaseDto {

    private String url;

    private String referer;

    private int amount;

    private String happenTime;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getReferer() {
        return referer;
    }

    public void setReferer(String referer) {
        this.referer = referer;
    }

    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }

    public String getHappenTime() {
        return happenTime;
    }

    public void setHappenTime(String happenTime) {
        this.happenTime = happenTime;
    }

}
