package com.renren.sns.guide.model;

import java.io.Serializable;

public class SameSchoolPeople implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = -3984719653697495095L;

	private int id;
	
	private String name;
	
	private String gender;
	
	private String tiny_url;
	
	private String collegeEnrollYear;
	
	private String highSchoolEnrollEnrollYear;
	
	private String juniorSchoolEnrollYear;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getGender() {
		return gender;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public String getTiny_url() {
		return tiny_url;
	}

	public void setTiny_url(String tiny_url) {
		this.tiny_url = tiny_url;
	}

	public String getCollegeEnrollYear() {
		return collegeEnrollYear;
	}

	public void setCollegeEnrollYear(String collegeEnrollYear) {
		this.collegeEnrollYear = collegeEnrollYear;
	}

	public String getHighSchoolEnrollEnrollYear() {
		return highSchoolEnrollEnrollYear;
	}

	public void setHighSchoolEnrollEnrollYear(String highSchoolEnrollEnrollYear) {
		this.highSchoolEnrollEnrollYear = highSchoolEnrollEnrollYear;
	}

	public String getJuniorSchoolEnrollYear() {
		return juniorSchoolEnrollYear;
	}

	public void setJuniorSchoolEnrollYear(String juniorSchoolEnrollYear) {
		this.juniorSchoolEnrollYear = juniorSchoolEnrollYear;
	}


	
}
