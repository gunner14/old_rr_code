package com.renren.sns.wiki.model.dict;

import java.io.Serializable;
import java.sql.Date;

/**
 * Wiki切词数据
 * 
 * @author yi.li@renren-inc.com since 2012-6-26
 * 
 */
public class WikiWord implements Serializable {

    private static final long serialVersionUID = 8751343074579675439L;

    private int id;

    private int wikiId;

    private String name;

    private int state;

    private Date createTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

}
