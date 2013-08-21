package com.xiaonei.page.xoa.domain;

import java.util.Date;

public class GossipViewX {
	private long id;
	private int guestId;
	private int chatId;
	private String chatName;
	private String guestName;
	private String guestUniv;
	private String body;
	private boolean isVip;
	private Date time;
	private String tinyUrl;
	private boolean online;
	private boolean keepUse;
	private boolean charity;
	private boolean superEmotion;
	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public int getGuestId() {
		return guestId;
	}
	public void setGuestId(int guestId) {
		this.guestId = guestId;
	}
	public int getChatId() {
		return chatId;
	}
	public void setChatId(int chatId) {
		this.chatId = chatId;
	}
	public String getChatName() {
		return chatName;
	}
	public void setChatName(String chatName) {
		this.chatName = chatName;
	}
	public String getGuestName() {
		return guestName;
	}
	public void setGuestName(String guestName) {
		this.guestName = guestName;
	}
	public String getGuestUniv() {
		return guestUniv;
	}
	public void setGuestUniv(String guestUniv) {
		this.guestUniv = guestUniv;
	}
	public String getBody() {
		return body;
	}
	public void setBody(String body) {
		this.body = body;
	}
	public boolean isVip() {
		return isVip;
	}
	public void setVip(boolean isVip) {
		this.isVip = isVip;
	}
	public Date getTime() {
		return time;
	}
	public void setTime(Date time) {
		this.time = time;
	}
	public String getTinyUrl() {
		return tinyUrl;
	}
	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}
	public boolean isOnline() {
		return online;
	}
	public void setOnline(boolean online) {
		this.online = online;
	}
	public boolean isKeepUse() {
		return keepUse;
	}
	public void setKeepUse(boolean keepUse) {
		this.keepUse = keepUse;
	}
	public boolean isSuperEmotion() {
		return superEmotion;
	}
	public void setSuperEmotion(boolean superEmotion) {
		this.superEmotion = superEmotion;
	}
	public boolean isCharity() {
		return charity;
	}
	public void setCharity(boolean charity) {
		this.charity = charity;
	}
	
	
}
