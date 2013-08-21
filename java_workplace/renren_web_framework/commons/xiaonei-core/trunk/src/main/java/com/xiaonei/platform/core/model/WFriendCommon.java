package com.xiaonei.platform.core.model;

import mop.hi.oce.domain.model.FriendCommon;

public class WFriendCommon extends FriendCommon{

	private String name;
	private String tinyurl;
	private String tinyFullUrl;
//	private String headUrl;
//	private String headFullUrl;
	
	
//	public String getHeadFullUrl() {
//		return headFullUrl;
//	}
//
//	public void setHeadFullUrl(String headFullUrl) {
//		this.headFullUrl = headFullUrl;
//	}
//
//	public String getHeadUrl() {
//		return headUrl;
//	}
//
//	public void setHeadUrl(String headUrl) {
//		this.headUrl = headUrl;
//	}

	public int getId(){
		return super.getUserId();
	}
	
	public void setId(int id){
		super.setUserId(id);
	}
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getTinyurl() {
		return tinyurl;
	}

	public void setTinyurl(String tinyurl) {
		this.tinyurl = tinyurl;
	}

	public String getTinyFullUrl() {
		return tinyFullUrl;
	}

	public void setTinyFullUrl(String tinyFullUrl) {
		this.tinyFullUrl = tinyFullUrl;
	}

}
