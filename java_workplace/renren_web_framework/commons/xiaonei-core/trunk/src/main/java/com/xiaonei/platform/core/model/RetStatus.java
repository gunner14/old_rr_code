package com.xiaonei.platform.core.model;

/**
 * 
 * RetStatus.java
 * @author drizzt drizzt.wang@gmail.com 2006-9-4 下午05:38:09
 */
public class RetStatus {
	int status = 0;
	String msg = "";
	
	public RetStatus(int stat) {
		this.status = stat;
	}
	
	public RetStatus(String msg) {
		this.msg = msg;
	}
	
	public RetStatus(int stat, String msg) {
		this.status = stat;
		this.msg = msg;
	}
	
	public String getMsg() {
		return msg;
	}
	public void setMsg(String msg) {
		this.msg = msg;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public String toString(){
		return "{status:"+this.status+",message:"+this.msg+"}" ;
	}
}