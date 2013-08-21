package com.renren.sns.guide.score;

import java.util.Date;

/**
 * 奖励明细
 * 
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * @since 2010-3-16
 */
public class AwardDetail implements Comparable<AwardDetail> {

    private int id;

    private int userId;

    private int type;

    private String awardKey;

    private String logDetail;

    private boolean opened;

    private Date updateTime;

    public Date getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Date updateTime) {
        this.updateTime = updateTime;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getAwardKey() {
        return awardKey;
    }

    public void setAwardKey(String awardKey) {
        this.awardKey = awardKey;
    }

    public String getLogDetail() {
        return logDetail;
    }

    public void setLogDetail(String logDetail) {
        this.logDetail = logDetail;
    }

    public boolean isOpened() {
        return opened;
    }

    public void setOpened(boolean opened) {
        this.opened = opened;
    }

    @Override
    public int compareTo(AwardDetail o) {
        if (this.updateTime == null || o.updateTime == null) return 0;
        return this.updateTime.compareTo(o.updateTime);
    }
    
    @Override
    public boolean equals(Object obj) {
    	return super.equals(obj);
    }
    
    @Override
    public int hashCode() {
    	return super.hashCode();
    }
}
