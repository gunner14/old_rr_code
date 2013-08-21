/**
 * 
 */
package com.xiaonei.platform.core.opt.feed.model;

import java.io.Serializable;

/**
 * @author 康伟 Nov 13, 2008 4:18:55 PM ver 1.0
 */
public final class FeedSendConfig implements Serializable{

	private static final long serialVersionUID = -6794639433646328194L;

	private int userid;

	private int type;

	private int minifeed;

	private int newsfeed;

	public int getMinifeed() {
		return minifeed;
	}

	public void setMinifeed(int minifeed) {
		this.minifeed = minifeed;
	}

	public int getNewsfeed() {
		return newsfeed;
	}

	public void setNewsfeed(int newsfeed) {
		this.newsfeed = newsfeed;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getUserid() {
		return userid;
	}

	public void setUserid(int userid) {
		this.userid = userid;
	}

}
