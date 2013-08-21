package com.xiaonei.platform.core.model;

import java.io.Serializable;


public class Workplace extends Network implements Serializable {

	private static final long serialVersionUID = -8326554102655494327L;

	private String email;

	private int countAll;

	private String building;

	public int getCountAll() {
		return countAll;
	}

	public void setCountAll(int countAll) {
		this.countAll = countAll;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getBuilding() {
		return building;
	}

	public void setBuilding(String building) {
		this.building = building;
	}

}
