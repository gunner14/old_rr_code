package com.renren.sns.wiki.model.feed;

import java.io.Serializable;

public class WikiFeedPhoto implements Serializable {

    private static final long serialVersionUID = 1L;

    private long photoId;

    private int ownerId;

    private String largeUrl;

    private int largeWidth;

    private int largeHeight;

    public long getPhotoId() {
        return photoId;
    }

    public void setPhotoId(long photoId) {
        this.photoId = photoId;
    }

    public int getOwnerId() {
        return ownerId;
    }

    public void setOwnerId(int ownerId) {
        this.ownerId = ownerId;
    }

    public String getLargeUrl() {
        return largeUrl;
    }

    public void setLargeUrl(String largeUrl) {
        this.largeUrl = largeUrl;
    }

    public int getLargeWidth() {
        return largeWidth;
    }

    public void setLargeWidth(int largeWidth) {
        this.largeWidth = largeWidth;
    }

    public int getLargeHeight() {
        return largeHeight;
    }

    public void setLargeHeight(int largeHeight) {
        this.largeHeight = largeHeight;
    }

}
