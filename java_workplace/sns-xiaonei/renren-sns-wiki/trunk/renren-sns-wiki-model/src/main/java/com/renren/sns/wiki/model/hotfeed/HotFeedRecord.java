package com.renren.sns.wiki.model.hotfeed;

import java.io.Serializable;
import java.sql.Date;

/**
 * 热门Feed信息
 * 
 * @author yi.li@renren-inc.com since 2012-4-24
 * 
 */
public class HotFeedRecord implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = -6953020035238989716L;

    private int wikiId;

    private long feedId;

    private int feedType;

    private int position;

    private Date updateTime;

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    public long getFeedId() {
        return feedId;
    }

    public void setFeedId(long feedId) {
        this.feedId = feedId;
    }

    public int getFeedType() {
        return feedType;
    }

    public void setFeedType(int feedType) {
        this.feedType = feedType;
    }

    public int getPosition() {
        return position;
    }

    public void setPosition(int position) {
        this.position = position;
    }

    public Date getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Date updateTime) {
        this.updateTime = updateTime;
    }

}
