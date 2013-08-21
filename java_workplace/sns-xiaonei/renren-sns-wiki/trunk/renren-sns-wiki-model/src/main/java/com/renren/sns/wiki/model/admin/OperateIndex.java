package com.renren.sns.wiki.model.admin;

import java.util.Date;

import com.renren.sns.wiki.model.Wiki;

/**
 * 运营首页对象
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
public class OperateIndex {

    private int id;

    private int wikiId;

    private int wikiType;

    private String operateType;

    private int order;

    private Date insertTime;
    
    private Wiki wiki;//用于辅助在前端显示数据

    
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

    
    public int getWikiType() {
        return wikiType;
    }

    
    public void setWikiType(int wikiType) {
        this.wikiType = wikiType;
    }

    
    public String getOperateType() {
        return operateType;
    }

    
    public void setOperateType(String operateType) {
        this.operateType = operateType;
    }

    
    public int getOrder() {
        return order;
    }

    
    public void setOrder(int order) {
        this.order = order;
    }

    
    public Date getInsertTime() {
        return insertTime;
    }

    
    public void setInsertTime(Date insertTime) {
        this.insertTime = insertTime;
    }


    
    public Wiki getWiki() {
        return wiki;
    }


    
    public void setWiki(Wiki wiki) {
        this.wiki = wiki;
    }
    
    

}
