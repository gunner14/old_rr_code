package com.xiaonei.recall_email.auto_login.model;

import java.text.DateFormat;
import java.text.ParseException;
import java.util.Date;

public class RecallEmailAutoLoginModel {

	private int userid;

	private String loginKey;

	private Date insertTime;

	private boolean isOverTime;

	public void set(RecallEmailAutoLoginModel model) {
		if (model != null) {
			userid = model.getUserid();
			loginKey = model.getLoginKey();
			insertTime = model.getInsertTime();
			isOverTime = model.isOverTime();
		}
	}

	public int getUserid() {
		return userid;
	}

	public RecallEmailAutoLoginModel setUserid(int userid) {
		this.userid = userid;
		return this;
	}

	public String getLoginKey() {
		return loginKey;
	}

	public RecallEmailAutoLoginModel setLoginKey(String loginKey) {
		this.loginKey = loginKey;
		return this;
	}

	public Date getInsertTime() {
		return insertTime;
	}

	public RecallEmailAutoLoginModel setInsertTime(Date insertTime) {
		this.insertTime = insertTime;
		return this;
	}

	public RecallEmailAutoLoginModel setInsertTime(String insertTime) {
		Date dInsertTime;
		try {
			dInsertTime = DateFormat.getDateTimeInstance().parse(insertTime);
		} catch (ParseException e) {
			e.printStackTrace();
			dInsertTime = new Date(System.currentTimeMillis());
		}
		this.insertTime = dInsertTime;
		return this;
	}

	public boolean isOverTime() {
		return isOverTime;
	}

	public RecallEmailAutoLoginModel setOverTime(boolean isOverTime) {
		this.isOverTime = isOverTime;
		return this;
	}

	public RecallEmailAutoLoginModel setOverTime(int isOverTime) {
		this.isOverTime = isOverTime == 0 ? false : true;
		return this;
	}

}
