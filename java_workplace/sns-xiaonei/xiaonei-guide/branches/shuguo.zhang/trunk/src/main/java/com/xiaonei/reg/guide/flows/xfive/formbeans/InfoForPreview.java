package com.xiaonei.reg.guide.flows.xfive.formbeans;

public class InfoForPreview {
	private String displayname;
	private String type;
	private String infoid;
	private String dbid;
	
	public String getDisplayname() {
		return displayname;
	}
	public void setDisplayname(String displayname) {
		this.displayname = displayname;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getInfoid() {
		return infoid;
	}
	public void setInfoid(String infoid) {
		this.infoid = infoid;
	}
	public String getDbid() {
		return dbid;
	}
	public void setDbid(String dbid) {
		this.dbid = dbid;
	}
	public int getUserid() {
		return userid;
	}
	public void setUserid(int userid) {
		this.userid = userid;
	}
	private int userid;
	
	//2009-11-25 12:00:52  infoyear 为满足诗伦的自动匹配年份的需求进行的增加 
	public int getInfoyear() {
		return infoyear;
	}
	public void setInfoyear(int infoyear) {
		this.infoyear = infoyear;
	}
	private int infoyear;
}
