package com.xiaonei.xnmonitor.domain;

import java.sql.Timestamp;

import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class UrlOvertime {

    private long id;

    private String domain;

    private String url;

    private Timestamp happenTime;

    private int costtime;

    private Timestamp analysisTime;

    @Id
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getDomain() {
        return domain;
    }

    public void setDomain(String domain) {
        this.domain = domain;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public Timestamp getHappenTime() {
        return happenTime;
    }

    public void setHappenTime(Timestamp happenTime) {
        this.happenTime = happenTime;
    }

    public int getCosttime() {
        return costtime;
    }

    public void setCosttime(int costtime) {
        this.costtime = costtime;
    }

    public Timestamp getAnalysisTime() {
        return analysisTime;
    }

    public void setAnalysisTime(Timestamp analysisTime) {
        this.analysisTime = analysisTime;
    }

}
