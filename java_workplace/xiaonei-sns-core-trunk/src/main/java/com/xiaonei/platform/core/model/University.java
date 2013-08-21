package com.xiaonei.platform.core.model;

import java.io.Serializable;


/**
 * 
 * University.java
 * Created by @author freebat (freebat@gmail.com) at 2005-8-30 16:28:12
 */
public class University extends Network implements Serializable {
	private static final long serialVersionUID = 4001252591245754735L;
	private String url = "";
	private int stuCount;
	private int nanCount;
	private int nvCount;
	
	private int starCount;
	private int annCount;
	private String logoUrl="univlogo/0/0.jpg";
	private int countryId;
	private int provinceId;
    

	public int getCountryId() {
		return countryId;
	}

	public void setCountryId(int countryId) {
		this.countryId = countryId;
	}

	public int getProvinceId() {
		return provinceId;
	}

	public void setProvinceId(int provinceId) {
		this.provinceId = provinceId;
	}

	public int getRegion() {
    	return this.getId() / 1000 * 1000;
    }
    
	

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	public int getNanCount() {
		return nanCount;
	}

	public void setNanCount(int nanCount) {
		this.nanCount = nanCount;
	}

	public int getNvCount() {
		return nvCount;
	}

	public void setNvCount(int nvCount) {
		this.nvCount = nvCount;
	}

	public int getStuCount() {
		return stuCount;
	}

	public void setStuCount(int stuCount) {
		this.stuCount = stuCount;
	}

	public int getStarCount() {
		return starCount;
	}

	public void setStarCount(int starCount) {
		this.starCount = starCount;
	}

	

	public int getAnnCount() {
		return annCount;
	}

	public void setAnnCount(int annCount) {
		this.annCount = annCount;
	}

	public String getLogoUrl() {
		return logoUrl;
	}

	public void setLogoUrl(String logoUrl) {
		this.logoUrl = logoUrl;
	}
}