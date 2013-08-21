package com.xiaonei.reg.guide.form;

import java.io.Serializable;

public class ShortFormFillInfoMajorForm extends ShortFormFillInfoBaseForm
        implements Serializable {

    private static final long serialVersionUID = -889245459559376415L;

    private String homeprovince;

    private String homecityName;

    private int homecityCode;

    private String isskip;

    private String porId;

    private String tinyUrl;

    private String headUrl;

    private String mainUrl;

    private String largeUrl;

    private String company;

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    public String getMainUrl() {
        return mainUrl;
    }

    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public boolean isSkip() {
        if ("true".equalsIgnoreCase(isskip))
            return true;
        return false;
    }

    public String getIsskip() {
        return isskip;
    }

    public void setIsskip(String isskip) {
        this.isskip = isskip;
    }

    public String getHomecityName() {
        return homecityName;
    }

    public void setHomecityName(String homecityName) {
        this.homecityName = homecityName;
    }

    public int getHomecityCode() {
        return homecityCode;
    }

    public void setHomecityCode(int homecityCode) {
        this.homecityCode = homecityCode;
    }

    public String getPorId() {
        return porId;
    }

    public void setPorId(String porId) {
        this.porId = porId;
    }

    public String getHomeprovince() {
        return homeprovince;
    }

    public void setHomeprovince(String homeprovince) {
        this.homeprovince = homeprovince;
    }

}
