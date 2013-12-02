package com.renren.renrengame.utils.applist;

public class GameItemInfo {

	public String ID = "";
	public String type = "";
	public String subtype = "";
	public String name = "";
	public String logo = "";
	public String logo75 = "";
	public String url = "";
	public String description = "";
	public String cavaspageurl = "";
	public String getCavaspageurl() {
		return cavaspageurl;
	}
	public void setCavaspageurl(String cavaspageurl) {
		this.cavaspageurl = cavaspageurl;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getLogo() {
		return logo;
	}
	public void setLogo(String logo) {
		this.logo = logo;
	}
	public String getUrl() {
		return url;
	}
	public void setUrl(String url) {
		this.url = url;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
}
