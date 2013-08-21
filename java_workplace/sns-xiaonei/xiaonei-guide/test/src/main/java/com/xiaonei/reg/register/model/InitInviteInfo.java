package com.xiaonei.reg.register.model;

import java.io.Serializable;

public class InitInviteInfo implements Serializable{
	private static final long serialVersionUID = -1404105722048641446L;
	private String name;
	private String headUrl;
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getHeadUrl() {
		return headUrl;
	}
	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}
	
}
