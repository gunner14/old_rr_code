package com.xiaonei.passport.web.model;

import java.util.HashMap;
import java.util.Map;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;

public class LoginResult {
	private LoginResponse loginResponse;
	private Map<String, Object> resultContents=new HashMap<String, Object>();
	public static LoginResult getDefualLoginResultWrapper(){
		LoginResult loginResultWrapper=new LoginResult(new LoginResponse());
		return loginResultWrapper;
	}
	public LoginResult(LoginResponse result){
		this.loginResponse=result;
	}
	

	public LoginResponse getLoginResponse() {
		return loginResponse;
	}
	public void setLoginResponse(LoginResponse loginResponse) {
		this.loginResponse = loginResponse;
	}
	public void setResultContents(Map<String, Object> resultContents) {
		this.resultContents = resultContents;
	}

	public Map<String, Object> getResultContents() {
		return resultContents;
	}
	public void addResultContents(String key, Object value) {
		this.resultContents.put(key, value);
	}
	public int getLoginStatusCode(){
		if(loginResponse!=null){
			return loginResponse.getLoginStatusCode();
		}
		return LoginStatusCode.FAILUNKNOWN.getCode();
	}
}
