package com.renren.sns.guide.model;

import java.io.Serializable;

public class GuideUserSomeInfo implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 980662319018868810L;

	private int userId;
	
	private int blogCount;
	
	private int albumCount;
	
	private int friendCount;
	
	private String middleSchoolName;
	
	private int middleSchoolYear;
	
	private String doingContent;
	
	private String htmlContentForPubLisher;

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getBlogCount() {
		return blogCount;
	}

	public void setBlogCount(int blogCount) {
		this.blogCount = blogCount;
	}

	public int getAlbumCount() {
		return albumCount;
	}

	public void setAlbumCount(int albumCount) {
		this.albumCount = albumCount;
	}

	public int getFriendCount() {
		return friendCount;
	}

	public void setFriendCount(int friendCount) {
		this.friendCount = friendCount;
	}

	public String getMiddleSchoolName() {
		return middleSchoolName;
	}

	public void setMiddleSchoolName(String middleSchoolName) {
		this.middleSchoolName = middleSchoolName;
	}

	public int getMiddleSchoolYear() {
		return middleSchoolYear;
	}

	public void setMiddleSchoolYear(int middleSchoolYear) {
		this.middleSchoolYear = middleSchoolYear;
	}

	public String getDoingContent() {
		return doingContent;
	}

	public void setDoingContent(String doingContent) {
		this.doingContent = doingContent;
	}

	public String getHtmlContentForPubLisher() {
		return htmlContentForPubLisher;
	}

	public void setHtmlContentForPubLisher(String htmlContentForPubLisher) {
		this.htmlContentForPubLisher = htmlContentForPubLisher;
	}
	
}
