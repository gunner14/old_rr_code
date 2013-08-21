package com.renren.sns.minisite.feed.xoa.model;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class BroadcastModel {

    private int siteId;

    private long ugcId;

    private long startTime = System.currentTimeMillis();

    private long notifyTime;

    private long activeCount;

    private long unActiveCount;

    private long activeFinished = 0L;

    private long unActiveFinished = 0L;

    private Log logger = LogFactory.getLog(this.getClass());

    /**
     * @return the siteId
     */
    public int getSiteId() {
        return this.siteId;
    }

    public long getUgcId() {
        return this.ugcId;
    }

    /**
     * @param siteId the siteId to set
     */
    public void setSiteId(final int siteId) {
        this.siteId = siteId;
    }

    public void setUgcId(final long ugcId) {
        this.ugcId = ugcId;
    }

    @Override
    public String toString() {
        return "BroadcastModel [siteId=" + this.siteId + ", ugcId=" + this.ugcId + "]";
    }

    public long getStartTime() {
        return startTime;
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public long getNotifyTime() {
        return notifyTime;
    }

    public void setNotifyTime(long notifyTime) {
        this.notifyTime = notifyTime;
    }

    public long getActiveCount() {
        return activeCount;
    }

    public void setActiveCount(long activeCount) {
        this.activeCount = activeCount;
    }

    public long getUnActiveCount() {
        return unActiveCount;
    }

    public void setUnActiveCount(long unActiveCount) {
        this.unActiveCount = unActiveCount;
    }

    public synchronized void incActiveFinished() {
        this.activeFinished++;
        if (this.activeFinished >= this.activeCount) {
            logger.info("ActiveNotifyFinished\t" + this.activeCount + "\t"
                    + (this.notifyTime - this.startTime) + "\t" +
                    (System.currentTimeMillis() - this.notifyTime));
        }
    }
    
    public synchronized void incUnActiveFinished() {
        this.unActiveFinished++;
        if (this.unActiveFinished >= this.unActiveCount) {
            logger.info("UnActiveNotifyFinished\t" + this.unActiveCount + "\t"
                    + (this.notifyTime - this.startTime) + "\t" +
                    (System.currentTimeMillis() - this.notifyTime));
        }
    }

}
