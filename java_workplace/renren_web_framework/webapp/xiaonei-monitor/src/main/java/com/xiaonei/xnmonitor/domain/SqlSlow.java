package com.xiaonei.xnmonitor.domain;

import java.sql.Timestamp;

import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class SqlSlow {

    private long id;

    private String sqlstr;

    private String serviceFrom;

    private Timestamp happenTime;

    private int costtime;

    private Timestamp endTime;

    private Timestamp analysisTime;

    @Id
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getSqlstr() {
        return sqlstr;
    }

    public void setSqlstr(String sqlstr) {
        this.sqlstr = sqlstr;
    }

    public String getServiceFrom() {
        return serviceFrom;
    }

    public void setServiceFrom(String serviceFrom) {
        this.serviceFrom = serviceFrom;
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

    public Timestamp getEndTime() {
        return endTime;
    }

    public void setEndTime(Timestamp endTime) {
        this.endTime = endTime;
    }

    public Timestamp getAnalysisTime() {
        return analysisTime;
    }

    public void setAnalysisTime(Timestamp analysisTime) {
        this.analysisTime = analysisTime;
    }

}
