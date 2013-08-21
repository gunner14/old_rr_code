package com.xiaonei.passport.model;


public class AdminGroupmember {

	private int adminid = 0;
	private String restrictIp;

	public int getAdminid() {
		return adminid;
	}

	public void setAdminid(int adminid) {
		this.adminid = adminid;
	}

	public String getRestrictIp() {
		return restrictIp;
	}

	public void setRestrictIp(String restrictIp) {
		this.restrictIp = restrictIp;
	}

}
