package com.renren.sns.wiki.model.rank;

import java.io.Serializable;
import java.util.Date;

/**
 * 用户积分
 * 
 * @author yi.li@renren-inc.com since 2012-5-16
 * 
 */
public class UserScore implements Serializable {

    private static final long serialVersionUID = -2372382693259534733L;

    private long id;

    private int wikiId;

    private int userId;

    private int score;

    private Date lastUpdateTime;

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

    public int getScore() {
        return score;
    }

    public void setScore(int score) {
        this.score = score;
    }

    public Date getLastUpdateTime() {
        return lastUpdateTime;
    }

    public void setLastUpdateTime(Date lastUpdateTime) {
        this.lastUpdateTime = lastUpdateTime;
    }
}
