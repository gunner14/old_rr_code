package com.dodoyo.invite.model;

/**
 * 还有哪些人邀请你加入过人人网的封装信息
 *
 * @author dream
 */
public class Owners {
    private String owner_name;

    private int friendsCount;

    private int notesCount;

    private int photosCount;

    private int blogsCount;

    public String getOwner_name() {
        return owner_name;
    }

    public void setOwner_name(String owner_name) {
        this.owner_name = owner_name;
    }

    public int getFriendsCount() {
        return friendsCount;
    }

    public void setFriendsCount(int friendsCount) {
        this.friendsCount = friendsCount;
    }

    public int getNotesCount() {
        return notesCount;
    }

    public void setNotesCount(int notesCount) {
        this.notesCount = notesCount;
    }

    public int getPhotosCount() {
        return photosCount;
    }

    public void setPhotosCount(int photosCount) {
        this.photosCount = photosCount;
    }

    public int getBlogsCount() {
        return blogsCount;
    }

    public void setBlogsCount(int blogsCount) {
        this.blogsCount = blogsCount;
    }

}