package com.renren.fmwidget.model;

public class WeatherData {

	private int nightTemperature;
	private String dayWeather;
	private int dayTemperature;
	private Date date;

	public int getNightTemperature() {
		return nightTemperature;
	}

	public void setNightTemperature(int nightTemperature) {
		this.nightTemperature = nightTemperature;
	}

	public String getDayWeather() {
		return dayWeather;
	}

	public void setDayWeather(String dayWeather) {
		this.dayWeather = dayWeather;
	}

	public int getDayTemperature() {
		return dayTemperature;
	}

	public void setDayTemperature(int dayTemperature) {
		this.dayTemperature = dayTemperature;
	}

	public Date getDate() {
		return date;
	}

	public void setDate(Date date) {
		this.date = date;
	}

	public static class Date {
		private String day_of_week;

		/**
		 * @param day_of_week
		 *            the day_of_week to set
		 */
		public void setDay_of_week(String day_of_week) {
			this.day_of_week = day_of_week;

		}

		/**
		 * @return the day_of_week
		 */
		public String getDay_of_week() {
			return day_of_week;
		}

	}
}