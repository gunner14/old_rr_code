package com.xiaonei.page.xoa.domain;

import java.util.Date;

public class DoingView {
    private long id;

    private int userId;

    private int commentCount;

    private int repeatCount;//下一级转发数

    private int repeatCountTotal;//所有子节点转发数

    private int markbit;//通用标志位

    private String content;

    private Date doTime;

    private int type;

    private String link;

    private String tinyUrl;

    private String ownerName;

    public String ip;

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getCommentCount() {
		return commentCount;
	}

	public void setCommentCount(int commentCount) {
		this.commentCount = commentCount;
	}

	public int getRepeatCount() {
		return repeatCount;
	}

	public void setRepeatCount(int repeatCount) {
		this.repeatCount = repeatCount;
	}

	public int getRepeatCountTotal() {
		return repeatCountTotal;
	}

	public void setRepeatCountTotal(int repeatCountTotal) {
		this.repeatCountTotal = repeatCountTotal;
	}

	public int getMarkbit() {
		return markbit;
	}

	public void setMarkbit(int markbit) {
		this.markbit = markbit;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public Date getDoTime() {
		return doTime;
	}

	public void setDoTime(Date doTime) {
		this.doTime = doTime;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public String getLink() {
		return link;
	}

	public void setLink(String link) {
		this.link = link;
	}

	public String getTinyUrl() {
		return tinyUrl;
	}

	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}

	public String getOwnerName() {
		return ownerName;
	}

	public void setOwnerName(String ownerName) {
		this.ownerName = ownerName;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

    

}
