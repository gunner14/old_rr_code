package com.renren.sns.wiki.model;

import java.util.Date;

/**
 * 
 * 保存用户操作相关的bean,包括 想读(取消) 读过(取消) 评分 的操作结果保存
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-5
 * 
 */
public class UserActionRecord {

    private long id;

    private int wikiId;

    private int userId;

    private String userName;

    private String userHeadurl;

    private String interest; //存放想 做 做过,值的保存参考豆瓣,分别为:wish do collect

    private Date interestTime;

    private int rating; //评分 1-5,默认值为0

    private Date ratingTime; //评分的时间

    private String comment; //简评

    private Date commentTime; //简评时间

    private Date actionTime; //数据插入时间

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserHeadurl() {
        return userHeadurl;
    }

    public void setUserHeadurl(String userHeadurl) {
        this.userHeadurl = userHeadurl;
    }

    public int getRating() {
        return rating;
    }

    public void setRating(int rating) {
        this.rating = rating;
    }

    public Date getRatingTime() {
        return ratingTime;
    }

    public void setRatingTime(Date ratingTime) {
        this.ratingTime = ratingTime;
    }

    public Date getActionTime() {
        return actionTime;
    }

    public void setActionTime(Date actionTime) {
        this.actionTime = actionTime;
    }

    public String getInterest() {
        return interest;
    }

    public void setInterest(String interest) {
        this.interest = interest;
    }

    public Date getInterestTime() {
        return interestTime;
    }

    public void setInterestTime(Date interestTime) {
        this.interestTime = interestTime;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public Date getCommentTime() {
        return commentTime;
    }

    public void setCommentTime(Date commentTime) {
        this.commentTime = commentTime;
    }

}
