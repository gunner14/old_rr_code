package com.renren.fmwidget.model;

import java.util.List;

public class WeatherContent {
	
	private List<WeatherData> data;
	
	private String city;

	public List<WeatherData> getData() {
		return data;
	}

	public void setData(List<WeatherData> data) {
		this.data = data;
	}

	public String getCity() {
		return city;
	}

	public void setCity(String city) {
		this.city = city;
	}
	
}
