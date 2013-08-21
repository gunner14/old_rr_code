package com.xiaonei.reg.guide.flows.xfive.formbeans;

import com.xiaonei.websecurity.adapter.SafeFilter;


public class FormBailing{

	private String stage;
	
	private String companyname;
	
	private String homeprovince;
	private String homecityname;
	private int homecitycode;
	
	private String currentprovince;

	private String currentcityname;

	private int currentcitycode;
	
	
	public String getInfo(){
		return " "+stage+"."+companyname+"."+homeprovince+"."+homecityname+"."+homecitycode+"."+currentprovince+"."+currentcityname+"."
		+currentcitycode+".";
	}

	public String getCurrentprovince() {
		return currentprovince;
	}
	public void setCurrentprovince(String currentprovince) {
		this.currentprovince = SafeFilter.getInstance().getPureContent(currentprovince);
	}
	public String getCurrentcityname() {
		return currentcityname;
	}
	public void setCurrentcityname(String currentcityname) {
		this.currentcityname = SafeFilter.getInstance().getPureContent(currentcityname);
	}
	public int getCurrentcitycode() {
		return currentcitycode;
	}
	public void setCurrentcitycode(int currentcitycode) {
		this.currentcitycode = currentcitycode;
	}
	public String getStage() {
		return stage;
	}
	public void setStage(String stage) {
		this.stage = stage;
	}
	public String getCompanyname() {
		return companyname;
	}
	public void setCompanyname(String companyname) {
		this.companyname = SafeFilter.getInstance().getPureContent(companyname);
	}
	public String getHomeprovince() {
		return homeprovince;
	}
	public void setHomeprovince(String homeprovince) {
		this.homeprovince = SafeFilter.getInstance().getPureContent(homeprovince);
	}
	public String getHomecityname() {
		return homecityname;
	}
	public void setHomecityname(String homecityname) {
		this.homecityname = SafeFilter.getInstance().getPureContent(homecityname);
	}
	public int getHomecitycode() {
		return homecitycode;
	}
	public void setHomecitycode(int homecitycode) {
		this.homecitycode = homecitycode;
	}

	
}
