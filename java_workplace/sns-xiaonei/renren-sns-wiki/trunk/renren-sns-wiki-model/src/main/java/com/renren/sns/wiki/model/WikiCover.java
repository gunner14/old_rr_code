package com.renren.sns.wiki.model;

import java.io.Serializable;

/**
 * 
 * wiki cover对应的类
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-31
 * 
 */
public class WikiCover implements Serializable {

    private static final long serialVersionUID = 8257269568451407143L;

    private int wikiId; // wiki id, 数据库的主键

    private long photoId; //照片id

    private String xlargeUrl; //原图

    private String largeUrl; //大图

    private String mainUrl; //中图

    private String headUrl; //头像

    private String tinyUrl; //小图

    private double xScale; //横比

    private double yScale; //纵比

    private double heightScale; //高

    private double widthScale; //宽

    private int xlargeUrlWidth; //xlarge_url宽度(像素数)

    private int xlargeUrlHeight; //xlarge_url高度(像素数)

    private String extraOption; //可选字段,如果是默认头图,设置为default

    
    public int getWikiId() {
        return wikiId;
    }

    
    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    
    public long getPhotoId() {
        return photoId;
    }

    
    public void setPhotoId(long photoId) {
        this.photoId = photoId;
    }

    
    public String getXlargeUrl() {
        return xlargeUrl;
    }

    
    public void setXlargeUrl(String xlargeUrl) {
        this.xlargeUrl = xlargeUrl;
    }

    
    public String getLargeUrl() {
        return largeUrl;
    }

    
    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    
    public String getMainUrl() {
        return mainUrl;
    }

    
    public void setMainUrl(String mainUrl) {
        this.mainUrl = mainUrl;
    }

    
    public String getHeadUrl() {
        return headUrl;
    }

    
    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }

    
    public String getTinyUrl() {
        return tinyUrl;
    }

    
    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    
    public double getxScale() {
        return xScale;
    }

    
    public void setxScale(double xScale) {
        this.xScale = xScale;
    }

    
    public double getyScale() {
        return yScale;
    }

    
    public void setyScale(double yScale) {
        this.yScale = yScale;
    }

    
    public double getHeightScale() {
        return heightScale;
    }

    
    public void setHeightScale(double heightScale) {
        this.heightScale = heightScale;
    }

    
    public double getWidthScale() {
        return widthScale;
    }

    
    public void setWidthScale(double widthScale) {
        this.widthScale = widthScale;
    }

    
    public int getXlargeUrlWidth() {
        return xlargeUrlWidth;
    }

    
    public void setXlargeUrlWidth(int xlargeUrlWidth) {
        this.xlargeUrlWidth = xlargeUrlWidth;
    }

    
    public int getXlargeUrlHeight() {
        return xlargeUrlHeight;
    }

    
    public void setXlargeUrlHeight(int xlargeUrlHeight) {
        this.xlargeUrlHeight = xlargeUrlHeight;
    }


    
    public String getExtraOption() {
        return extraOption;
    }


    
    public void setExtraOption(String extraOption) {
        this.extraOption = extraOption;
    }

    
    
}
