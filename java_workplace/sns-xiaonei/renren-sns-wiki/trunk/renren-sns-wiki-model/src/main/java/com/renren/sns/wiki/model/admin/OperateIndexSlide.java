package com.renren.sns.wiki.model.admin;

import java.util.Date;

import com.renren.sns.wiki.model.Wiki;

/**
 * 首页运营slide的bean
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-21
 * 
 */
public class OperateIndexSlide {

    private int id;

    private int wikiId;

    private int wikiType;

    private String imageUrl;

    private int imageWidth;

    private int imageHeight;

    private int order;

    private Date insertTime;
    
    private Wiki wiki;

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

    public String getImageUrl() {
        return imageUrl;
    }

    public void setImageUrl(String imageUrl) {
        this.imageUrl = imageUrl;
    }

    public int getImageWidth() {
        return imageWidth;
    }

    public void setImageWidth(int imageWidth) {
        this.imageWidth = imageWidth;
    }

    public int getImageHeight() {
        return imageHeight;
    }

    public void setImageHeight(int imageHeight) {
        this.imageHeight = imageHeight;
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
