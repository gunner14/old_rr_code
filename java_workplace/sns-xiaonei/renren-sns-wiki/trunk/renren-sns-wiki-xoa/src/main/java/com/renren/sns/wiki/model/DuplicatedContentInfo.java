package com.renren.sns.wiki.model;

import java.io.Serializable;

/**
 * 用来存储
 * 
 * @author yi.li@renren-inc.com since Aug 1, 2012
 * 
 */
public class DuplicatedContentInfo implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = 7258246656826145252L;

    private long ugcId;

    private int feedType;

    private int ownerId;

    public long getUgcId() {
        return ugcId;
    }

    public void setUgcId(long ugcId) {
        this.ugcId = ugcId;
    }

    public int getFeedType() {
        return feedType;
    }

    public void setFeedType(int feedType) {
        this.feedType = feedType;
    }

    public int getOwnerId() {
        return ownerId;
    }

    public void setOwnerId(int ownerId) {
        this.ownerId = ownerId;
    }
}
