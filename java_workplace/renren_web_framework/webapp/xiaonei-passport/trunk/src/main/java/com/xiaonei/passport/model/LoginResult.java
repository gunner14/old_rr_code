package com.xiaonei.passport.model;

import java.io.Serializable;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;

public class LoginResult implements Serializable{
	private static final long serialVersionUID = 1L;
	private int userId=-1;
	private int loginStatusCode=0;
	private String loginStatusdescription;
	private String passportTicket="";
	private String ticket=null;
	private String account="";
	private int status=-1;
	private String domain="renren.com";
	private int realUserId=-1;
	private int catchaCount;
	private boolean isWeakPassword=false;
	private boolean gotoSafeCenter=false;
	private String oldPassport;
	private String oldTicket;
	private boolean sendMail=false;
	private transient LoginStatusCode code;
	//private int status;
	/**
	 * 出错时调用
	 */
	public LoginResult(LoginStatusCode code, String account, String domain){
		this.loginStatusCode=code.getCode();
		this.loginStatusdescription=code.getDescription();
		this.account=account;
		this.code=code;
		this.domain=domain;
	}
	/**
	 * 成功时调用
	 */
	public LoginResult(LoginStatusCode code, String account, int userId, int realUserId, int status,String passportTicket, String ticket, String domain){
		this.loginStatusCode=code.getCode();
		this.loginStatusdescription=code.getDescription();
		this.account=account;
		this.userId=userId;
		this.realUserId=realUserId;
		this.passportTicket=passportTicket;
		this.ticket=ticket;
		this.domain=domain;
		this.code=code;
		this.status=status;
	}
	public LoginResponse createLoginResponse(){
		LoginResponse result=new LoginResponse();
		result.setAccount(account);
		result.setCatchaCount(catchaCount);
		result.setDomain(domain);
		result.setGotoSafeCenter(gotoSafeCenter);
		result.setLoginStatusCode(loginStatusCode);
		result.setLoginStatusdescription(loginStatusdescription);
		result.setOldPassport(oldPassport);
		result.setOldTicket(oldTicket);
		result.setPassportTicket(passportTicket);
		result.setRealUserId(realUserId);
		result.setStatus(loginStatusCode);
		result.setTicket(ticket);
		result.setUserId(userId);
		result.setWeakPassword(isWeakPassword);
		result.setSendMail(sendMail);
		return result;
	}
	public int getLoginStatusCode() {
		return loginStatusCode;
	}
	public String getLoginStatusdescription() {
		return loginStatusdescription;
	}
	public String getPassportTicket() {
		return passportTicket;
	}
	public String getTicket() {
		return ticket;
	}
	public String getAccount() {
		return account;
	}
	public String getDomain() {
		return domain;
	}
	
	public int getUserId() {
		return userId;
	}
	public int getRealUserId() {
		return realUserId;
	}
	public void setLoginStatusCode(LoginStatusCode code){
		this.loginStatusCode=code.getCode();
		this.loginStatusdescription=code.getDescription();
		this.code=code;
	}
	public int getCatchaCount() {
		return catchaCount;
	}
	public void setCatchaCount(int catchaCount) {
		this.catchaCount = catchaCount;
	}
	public LoginStatusCode getCode() {
		return code;
	}
	public boolean isWeakPassword() {
		return isWeakPassword;
	}
	public void setWeakPassword(boolean isWeakPassword) {
		this.isWeakPassword = isWeakPassword;
	}
	public int getStatus() {
		return status;
	}
	public boolean isGotoSafeCenter() {
		return gotoSafeCenter;
	}
	public void setGotoSafeCenter(boolean gotoSafeCenter) {
		this.gotoSafeCenter = gotoSafeCenter;
	}
	public String getOldPassport() {
		return oldPassport;
	}
	public void setOldPassport(String oldPassport) {
		this.oldPassport = oldPassport;
	}
	public String getOldTicket() {
		return oldTicket;
	}
	public void setOldTicket(String oldTicket) {
		this.oldTicket = oldTicket;
	}
	public boolean isSendMail() {
		return sendMail;
	}
	public void setSendMail(boolean sendMail) {
		this.sendMail = sendMail;
	}
	
}
