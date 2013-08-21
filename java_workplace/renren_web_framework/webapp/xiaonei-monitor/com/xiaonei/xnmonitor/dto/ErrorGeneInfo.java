package com.xiaonei.xnmonitor.dto;

import java.sql.Timestamp;
import java.text.Format;
import java.text.SimpleDateFormat;

public class ErrorGeneInfo extends BaseDto {

    private String domain;

    private int count404;

    private int count500;

    public String getDomain() {
        return domain;
    }

    public void setDomain(String domain) {
        this.domain = domain;
    }

    public int getCount404() {
        return count404;
    }

    public void setCount404(int count404) {
        this.count404 = count404;
    }

    public int getCount500() {
        return count500;
    }

    public void setCount500(int count500) {
        this.count500 = count500;
    }

}
