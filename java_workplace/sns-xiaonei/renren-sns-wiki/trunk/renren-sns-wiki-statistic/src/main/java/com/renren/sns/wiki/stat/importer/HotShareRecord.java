package com.renren.sns.wiki.stat.importer;

/**
 * 代表每一条Hadoop跑出来的记录
 * 
 * @author yi.li@renren-inc.com since 2012-6-26
 * 
 */
public class HotShareRecord {

    private long sourceId;

    private long albumId;

    private int ownerId;

    private int stype;

    private int shareCount;

    public long getSourceId() {
        return sourceId;
    }

    public void setSourceId(long sourceId) {
        this.sourceId = sourceId;
    }

    public int getOwnerId() {
        return ownerId;
    }

    public void setOwnerId(int ownerId) {
        this.ownerId = ownerId;
    }

    public int getStype() {
        return stype;
    }

    public void setStype(int stype) {
        this.stype = stype;
    }

    public int getShareCount() {
        return shareCount;
    }

    public void setShareCount(int shareCount) {
        this.shareCount = shareCount;
    }

    public long getAlbumId() {
        return albumId;
    }

    public void setAlbumId(long albumId) {
        this.albumId = albumId;
    }

    public String toString() {
        return this.getStype() + "-" + this.getShareCount();
    }

}
