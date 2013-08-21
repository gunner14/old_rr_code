package com.renren.sns.wiki.model.rank;

import java.io.Serializable;
import java.util.Date;

/**
 * Wiki积分排名纪录
 * 
 * @author yi.li@renren-inc.com since 2012-5-16
 * 
 */
public class WikiRankRecord implements Serializable {

    private static final long serialVersionUID = -6712943567654827816L;

    private long id;

    private int wikiId;

    private int score;

    private int capacity;

    private int rank;

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

    public int getScore() {
        return score;
    }

    public void setScore(int score) {
        this.score = score;
    }

    public int getCapacity() {
        return capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public Date getLastUpdateTime() {
        return lastUpdateTime;
    }

    public void setLastUpdateTime(Date lastUpdateTime) {
        this.lastUpdateTime = lastUpdateTime;
    }

}
