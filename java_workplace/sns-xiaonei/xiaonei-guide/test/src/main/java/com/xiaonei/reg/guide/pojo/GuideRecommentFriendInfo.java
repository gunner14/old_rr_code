package com.xiaonei.reg.guide.pojo;

import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.model.User;

public class GuideRecommentFriendInfo {
	
	private String sex;
	private String school;
	private String province;
	private String city;
	private String name;
	private WUserCache wUserCache;
	private User user;
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
	
	public String getSchool() {
		return school;
	}
	public void setSchool(String school) {
		this.school = school;
	}
	public String getProvince() {
		return province;
	}
	public void setProvince(String province) {
		this.province = province;
	}
	public String getCity() {
		return city;
	}
	public void setCity(String city) {
		this.city = city;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public WUserCache getwUserCache() {
		return wUserCache;
	}
	public void setwUserCache(WUserCache wUserCache) {
		this.wUserCache = wUserCache;
	}
	public User getUser() {
		return user;
	}
	public void setUser(User user) {
		this.user = user;
	}
	
	
	
	

}
