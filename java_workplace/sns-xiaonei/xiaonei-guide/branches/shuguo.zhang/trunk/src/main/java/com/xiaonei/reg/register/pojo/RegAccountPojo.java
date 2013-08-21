package com.xiaonei.reg.register.pojo;

import java.io.Serializable;

import org.apache.commons.lang.StringUtils;

/**
 * 建立账户信息时所使用的数据
 * 
 * @author wang-tai
 */
public class RegAccountPojo implements Serializable {

	private static final long serialVersionUID = -61166832231333925L;

	private String account;
	private String pwd;
	private String name;
	private int netid;
	private int stage;
	private String ip;
	private String gender;
	private String msn;
	private String highSchoolCode;
	private String highSchoolName;
	private String tinyUrl;
	private String headUrl;
	private String mainUrl;
	private String largeUrl;
	private int birthYear;
	private int birthMonth;
	private int birthDay;

	public String toString() {
		return account + " " + pwd + " " + name + " " + netid + " " + stage
				+ " " + ip + " " + gender + " " + msn + " " + highSchoolCode
				+ " " + highSchoolName + " " + tinyUrl;
	}

	public int getBirthYear() {
		return birthYear;
	}

	public void setBirthYear(String birthYear) {
		try {
			this.birthYear = Integer.valueOf(birthYear);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

	public int getBirthMonth() {
		return birthMonth;
	}

	public void setBirthMonth(String birthMonth) {
		try {
			this.birthMonth = Integer.valueOf(birthMonth);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

	public int getBirthDay() {
		return birthDay;
	}

	public void setBirthDay(String birthDay) {
		try {
			this.birthDay = Integer.valueOf(birthDay);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

	public String getMainUrl() {
		return mainUrl;
	}

	public void setMainUrl(String mainUrl) {
		this.mainUrl = mainUrl;
	}

	public String getAccount() {
		return StringUtils.trimToEmpty(account).toLowerCase();
	}

	public String getGender() {
		return gender;
	}

	public String getHeadUrl() {
		return headUrl;
	}

	public String getHighSchoolCode() {
		return highSchoolCode;
	}

	public String getHighSchoolName() {
		return highSchoolName;
	}

	public String getIp() {
		return ip;
	}

	public String getMsn() {
		return msn;
	}

	public String getName() {
		return name;
	}

	public int getNetid() {
		return netid;
	}

	public String getPwd() {
		return pwd;
	}

	public int getStage() {
		return stage;
	}

	public String getTinyUrl() {
		return tinyUrl;
	}

	public void setAccount(String account) {
		this.account = account;
	}

	public void setGender(String gender) {
		this.gender = gender;
	}

	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}

	public void setHighSchoolCode(String highSchoolCode) {
		this.highSchoolCode = highSchoolCode;
	}

	public void setHighSchoolName(String highSchoolName) {
		this.highSchoolName = highSchoolName;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public void setMsn(String msn) {
		this.msn = msn;
	}

	public void setName(String name) {
		this.name = name;
	}

	public void setNetid(int netid) {
		this.netid = netid;
	}

	public void setPwd(String pwd) {
		this.pwd = pwd;
	}

	public void setStage(int stage) {
		this.stage = stage;
	}

	public void setTinyUrl(String tinyUrl) {
		this.tinyUrl = tinyUrl;
	}

	public void setLargeUrl(String largeUrl) {
		this.largeUrl = largeUrl;
	}

	public String getLargeUrl() {
		return largeUrl;
	}

}
