package com.xiaonei.xnmonitor.dto;

import java.net.URLEncoder;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class SqlSlowInfo extends BaseDto {

    private String serviceFrom;

    private double avgTime;

    private int times;

    private double maxTime;

    private double minTime;

    private String sqlstr;

    private String sqlstrEncode;

    public String getServiceFrom() {
        return serviceFrom;
    }

    public void setServiceFrom(String serviceFrom) {
        this.serviceFrom = serviceFrom;
    }

    public double getAvgTime() {
        return avgTime;
    }

    public void setAvgTime(double avgTime) {
        this.avgTime = avgTime;
    }

    public int getTimes() {
        return times;
    }

    public void setTimes(int times) {
        this.times = times;
    }

    public double getMaxTime() {
        return maxTime;
    }

    public void setMaxTime(double maxTime) {
        this.maxTime = maxTime;
    }

    public double getMinTime() {
        return minTime;
    }

    public void setMinTime(double minTime) {
        this.minTime = minTime;
    }

    public String getSqlstr() {
        return sqlstr;
    }

    public void setSqlstr(String sqlstr) {
        this.sqlstr = sqlstr;
    }

    public String getSqlstrEncode() {
        return sqlstrEncode;
    }

    public void setSqlstrEncode(String sqlstrEncode) {
        this.sqlstrEncode = URLEncoder.encode(sqlstrEncode);
    }

}
