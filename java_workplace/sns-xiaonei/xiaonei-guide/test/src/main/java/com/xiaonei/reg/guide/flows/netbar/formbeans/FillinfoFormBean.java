package com.xiaonei.reg.guide.flows.netbar.formbeans;

public class FillinfoFormBean {

	private String currentprovince;
	private String currentcityname;
	private int currentcitycode;
	private String homeprovince;
	private String homecityname;
	private int homecitycode;
	private String whocanseeme;
	public static String [] getProps(){
		return new String [] {
				"currentprovince",
				"currentcityname",
				"currentcitycode",
				"homeprovince",
				"homecityname",
				"homecitycode",
				};
	}
	public String getCurrentprovince() {
		return currentprovince;
	}
	public void setCurrentprovince(String currentprovince) {
		this.currentprovince = currentprovince;
	}
	public String getCurrentcityname() {
		return currentcityname;
	}
	public void setCurrentcityname(String currentcityname) {
		this.currentcityname = currentcityname;
	}
	public int getCurrentcitycode() {
		return currentcitycode;
	}
	public void setCurrentcitycode(int currentcitycode) {
		this.currentcitycode = currentcitycode;
	}
	public String getHomeprovince() {
		return homeprovince;
	}
	public void setHomeprovince(String homeprovince) {
		this.homeprovince = homeprovince;
	}
	public String getHomecityname() {
		return homecityname;
	}
	public void setHomecityname(String homecityname) {
		this.homecityname = homecityname;
	}
	public int getHomecitycode() {
		return homecitycode;
	}
	public void setHomecitycode(int homecitycode) {
		this.homecitycode = homecitycode;
	}
	public String getWhocanseeme() {
		return whocanseeme;
	}
	public void setWhocanseeme(String whocanseeme) {
		this.whocanseeme = whocanseeme;
	}

}
