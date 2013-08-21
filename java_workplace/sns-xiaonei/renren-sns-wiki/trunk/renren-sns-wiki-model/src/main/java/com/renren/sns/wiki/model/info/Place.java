package com.renren.sns.wiki.model.info;

import com.renren.sns.wiki.model.info.annotation.WikiInfoFieldName;

/**
 * 
 * 位置/简介 Wiki页资料收起状态默认展示：简介
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 * 
 */
public class Place extends WikiInfo {

    @WikiInfoFieldName( { "别名", "外文名", "英文名" })
    private String anotherName;

    @WikiInfoFieldName( { "地址", "位置" })
    private String location;//位置

    @WikiInfoFieldName( { "景点级别" })
    private String level;

    @WikiInfoFieldName( { "门票价格" })
    private String ticketPrice;

    @WikiInfoFieldName( { "简介" })
    private String summary;//简介

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    public String getAnotherName() {
        return anotherName;
    }

    public void setAnotherName(String anotherName) {
        this.anotherName = anotherName;
    }

    public String getLevel() {
        return level;
    }

    public void setLevel(String level) {
        this.level = level;
    }

    public String getTicketPrice() {
        return ticketPrice;
    }

    public void setTicketPrice(String ticketPrice) {
        this.ticketPrice = ticketPrice;
    }

}
