package com.xiaonei.platform.core.model;

import mop.hi.oce.domain.model.FriendShare;

public class WFriendShare extends FriendShare {

	public String name;
	public String tinyurl;
	public String tinyFullUrl;
	
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
