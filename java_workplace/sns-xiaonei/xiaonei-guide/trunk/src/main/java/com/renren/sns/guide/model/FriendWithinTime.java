package com.renren.sns.guide.model;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-7-5 上午11:58:49
 */
public class FriendWithinTime {

    private int userId;

    private String userName;

    private String userTinyUrl;

    private int friendCount;

    private int albumCount;

    private int blogCount;

    private int doingCount;
    
    private long doingId;
    
    private String doingTime;

    private String newestDoing;
    
    private boolean isFriend;

    private boolean showProfile;
    
    private boolean showGossipBoard;

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

    public String getUserTinyUrl() {
        return userTinyUrl;
    }

    public void setUserTinyUrl(String userTinyUrl) {
        this.userTinyUrl = userTinyUrl;
    }

    public int getFriendCount() {
        return friendCount;
    }

    public void setFriendCount(int friendCount) {
        this.friendCount = friendCount;
    }

    public int getAlbumCount() {
        return albumCount;
    }

    public void setAlbumCount(int albumCount) {
        this.albumCount = albumCount;
    }

    public int getBlogCount() {
        return blogCount;
    }

    public void setBlogCount(int blogCount) {
        this.blogCount = blogCount;
    }

    public int getDoingCount() {
        return doingCount;
    }

    public void setDoingCount(int doingCount) {
        this.doingCount = doingCount;
    }
    
    public long getDoingId() {
        return doingId;
    }

    public void setDoingId(long doingId) {
        this.doingId = doingId;
    }
    
    public String getDoingTime() {
        return doingTime;
    }

    public void setDoingTime(String doingTime) {
        this.doingTime = doingTime;
    }

    public String getNewestDoing() {
        return newestDoing;
    }

    public void setNewestDoing(String newestDoing) {
        this.newestDoing = newestDoing;
    }

    public void setFriend(boolean isFriend) {
        this.isFriend = isFriend;
    }

    public boolean isFriend() {
        return isFriend;
    }
    
    public boolean isShowProfile() {
        return showProfile;
    }
    
    public void setShowProfile(boolean showProfile) {
        this.showProfile = showProfile;
    }
    
    public boolean isShowGossipBoard() {
        return showGossipBoard;
    }
    
    public void setShowGossipBoard(boolean showGossipBoard) {
        this.showGossipBoard = showGossipBoard;
    }

}
