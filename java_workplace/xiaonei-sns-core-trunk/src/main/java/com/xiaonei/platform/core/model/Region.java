package com.xiaonei.platform.core.model;

import java.io.Serializable;


public class Region extends Network implements Serializable {
	private static final long serialVersionUID = 4001252591245754735L;
    
    private int provinceId ;
    private int countA ;
	public int getCountA() {
		return countA;
	}
	public void setCountA(int countA) {
		this.countA = countA;
	}
	
	public int getProvinceId() {
		return provinceId;
	}
	public void setProvinceId(int provinceId) {
		this.provinceId = provinceId;
	}
}
