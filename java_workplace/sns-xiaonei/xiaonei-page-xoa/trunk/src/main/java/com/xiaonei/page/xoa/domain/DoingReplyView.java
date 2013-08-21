package com.xiaonei.page.xoa.domain;

import java.util.Date;

public class DoingReplyView {
	private long id;
	private long doingId;
	private int ownerId;
	private String ownerName;
	private int replyerId;
	private String replyContent;
	private String replyerName;
	private String replyerTinyurl;
	private Date replyTime;
	private String meta;
	
	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public long getDoingId() {
		return doingId;
	}
	public void setDoingId(long doingId) {
		this.doingId = doingId;
	}
	public int getOwnerId() {
		return ownerId;
	}
	public void setOwnerId(int ownerId) {
		this.ownerId = ownerId;
	}
	public String getOwnerName() {
		return ownerName;
	}
	public void setOwnerName(String ownerName) {
		this.ownerName = ownerName;
	}
	public int getReplyerId() {
		return replyerId;
	}
	public void setReplyerId(int replyerId) {
		this.replyerId = replyerId;
	}
	public String getReplyContent() {
		return replyContent;
	}
	public void setReplyContent(String replyContent) {
		this.replyContent = replyContent;
	}
	public String getReplyerName() {
		return replyerName;
	}
	public void setReplyerName(String replyerName) {
		this.replyerName = replyerName;
	}
	public String getReplyerTinyurl() {
		return replyerTinyurl;
	}
	public void setReplyerTinyurl(String replyerTinyurl) {
		this.replyerTinyurl = replyerTinyurl;
	}
	public Date getReplyTime() {
		return replyTime;
	}
	public void setReplyTime(Date replyTime) {
		this.replyTime = replyTime;
	}
	public String getMeta() {
		return meta;
	}
	public void setMeta(String meta) {
		this.meta = meta;
	}
	
	
}
