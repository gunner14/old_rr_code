package com.xiaonei.xnmonitor.dto;

public class IMInfo {

    private long id;

    private String type;

    private long costTime;

    private String urlOrHost;

    private String happenTime;

    private int total;

    private double averageCost;

    private String IP;

    private Double variance;

    private Double percent;

    private String location;

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public long getCostTime() {
        return costTime;
    }

    public void setCostTime(long costTime) {
        this.costTime = costTime;
    }

    public String getUrlOrHost() {
        return urlOrHost;
    }

    public void setUrlOrHost(String urlOrHost) {
        this.urlOrHost = urlOrHost;
    }

    public String getHappenTime() {
        return happenTime;
    }

    public void setHappenTime(String happenTime) {
        this.happenTime = happenTime;
    }

    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }

    public double getAverageCost() {
        return averageCost;
    }

    public void setAverageCost(double averageCost) {
        this.averageCost = averageCost;
    }

    public String getIP() {
        return IP;
    }

    public void setIP(String iP) {
        IP = iP;
    }

    public Double getVariance() {
        return variance;
    }

    public void setVariance(Double variance) {
        this.variance = variance;
    }

    public Double getPercent() {
        return percent;
    }

    public void setPercent(Double percent) {
        this.percent = percent;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

}
