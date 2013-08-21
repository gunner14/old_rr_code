package com.renren.sns.guide.model;

import java.io.Serializable;

public class GuideCity implements Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -1371193499636763291L;

	private String cityCode;

	private String cityName;
	
	public String getCityCode() {
		return cityCode;
	}

	public void setCityCode(String cityCode) {
		this.cityCode = cityCode;
	}

	public String getCityName() {
		return cityName;
	}

	public void setCityName(String cityName) {
		this.cityName = cityName;
	}
	
}
