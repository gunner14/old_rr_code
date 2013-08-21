package com.renren.sns.wiki.model;

import java.io.Serializable;
import java.util.Date;

/**
 * 主要的wiki model对象
 * 
 * @author yi.li@renren-inc.com since 2012-3-23
 * 
 */
public class Wiki implements Serializable {

    private static final long serialVersionUID = 8313093184632891295L;

    private int id; // wiki id

    private String name; // wiki 名字

    private String alias; // wiki 别名 - 用于Url后缀

    private long photoId; // wiki 头像照片id

    private String tinyUrl;

    private String headUrl;

    private String mainUrl;

    private String largeUrl;

    private int likeCount; // wiki 喜欢数

    private int wantCount; // wiki 想做某事的数量

    private int enjoyingCount; // wiki 在做某事的数量

    private int enjoyedCount; // wiki 做过某事的数量

    private int valuationCount; // 评分人数

    private int valuationScore; // 评分总分

    private int type; // wiki类型

    private int subType; // wiki子分类

    private int state; // wiki状态

    private Date createTime; // 创建时间

    private int creator; // 创建人

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAlias() {
        return alias;
    }

    public void setAlias(String alias) {
        this.alias = alias;
    }

    public long getPhotoId() {
        return photoId;
    }

    public void setPhotoId(long photoId) {
        this.photoId = photoId;
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

    public int getLikeCount() {
        return likeCount;
    }

    public void setLikeCount(int likeCount) {
        this.likeCount = likeCount;
    }

    public int getWantCount() {
        return wantCount;
    }

    public void setWantCount(int wantCount) {
        this.wantCount = wantCount;
    }

    public int getEnjoyingCount() {
        return enjoyingCount;
    }

    public void setEnjoyingCount(int enjoyingCount) {
        this.enjoyingCount = enjoyingCount;
    }

    public int getEnjoyedCount() {
        return enjoyedCount;
    }

    public void setEnjoyedCount(int enjoyedCount) {
        this.enjoyedCount = enjoyedCount;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getSubType() {
        return subType;
    }

    public void setSubType(int subType) {
        this.subType = subType;
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

    public int getCreator() {
        return creator;
    }

    public void setCreator(int creator) {
        this.creator = creator;
    }

    public int getValuationCount() {
        return valuationCount;
    }

    public void setValuationCount(int valuationCount) {
        this.valuationCount = valuationCount;
    }

    public int getValuationScore() {
        return valuationScore;
    }

    public void setValuationScore(int valuationScore) {
        this.valuationScore = valuationScore;
    }
}
