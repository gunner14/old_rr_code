package com.renren.xoa.commons.test;

import java.io.Serializable;

public class Blog implements Serializable {

	private static final long serialVersionUID = 1L;

	private int ownerId;
	
	private String title;
	
	private String content;

	public int getOwnerId() {
		return ownerId;
	}

	public void setOwnerId(int ownerId) {
		this.ownerId = ownerId;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}
	
	public String toString() {
		return "ownerId=" + ownerId + ", title=" + title + ", content=" + content;
	}
	
}
