package com.xiaonei.passport.model;

import java.io.Serializable;

import com.renren.passport.service.LogoutResponse;
import com.xiaonei.passport.constants.LogoutStatusCode;


public class LogoutResult implements Serializable{
	private static final long serialVersionUID = 1L;
	private int logoutStatusCode=0;
	private String logoutStatusdescription;
	public LogoutResult(LogoutStatusCode code){
		this.logoutStatusCode=code.getCode();
		this.logoutStatusdescription=code.getDescription();
	}
	public LogoutResponse createLogoutResponse(){
		LogoutResponse result=new LogoutResponse();
		result.setLogoutStatusCode(logoutStatusCode);
		result.setLogoutStatusdescription(logoutStatusdescription);
		return result;
	}
	public int getLogoutStatusCode() {
		return logoutStatusCode;
	}
	public String getLogoutStatusdescription() {
		return logoutStatusdescription;
	}
	
	
}
