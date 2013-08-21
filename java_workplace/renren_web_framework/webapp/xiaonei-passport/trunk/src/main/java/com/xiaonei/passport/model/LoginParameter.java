package com.xiaonei.passport.model;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.platform.core.utility.RegularExpression;

public class LoginParameter implements ILoginParameter{
	private String userName; 
	private String md5Password;
	private boolean isAutoLogin=false;
	private String captchaCode=null;
	private	String captchaType=null;
	private String ick=null;
	private String domain;
	private long expirePeriod;
	private int loginRegistrationId;
	private boolean createRenRenTicket=true;
	private  ILoginContext context;
	
	public LoginParameter(String userName, String md5Password, boolean isAutoLogin, String icode,
			String captchaType, String ick,String domain,
			long expirePeriod, int loginRegistrationId, 
			boolean createRenRenTicket, final ILoginContext context){
		this.userName=getSecurityAccount(userName);
		this.md5Password=md5Password;
		this.isAutoLogin=isAutoLogin;
		this.captchaCode=icode;
		this.captchaType=captchaType;
		this.ick=ick;
		this.domain=domain;
		this.expirePeriod=expirePeriod;
		this.loginRegistrationId=loginRegistrationId;
		this.createRenRenTicket=createRenRenTicket;
		this.context=context;
	}
	
	public String getUserName() {
		return userName;
	}
	public String getMd5Password() {
		return md5Password;
	}
	public boolean isAutoLogin() {
		return isAutoLogin;
	}
	public String getCaptchaCode() {
		return captchaCode;
	}
	public String getCaptchaType() {
		return captchaType;
	}
	public String getIck() {
		return ick;
	}
	public long getExpirePeriod() {
		return expirePeriod;
	}
	public int getLoginRegistrationId() {
		return loginRegistrationId;
	}
	@Override
	public ILoginContext getLogincontext() {
		return context;
	}
	@Override
	public boolean needCreateRenRenTicket() {
		return createRenRenTicket;
	}
	
	public String getDomain() {
		return domain;
	}

	private String getSecurityAccount(String account){
		String email = StringUtils.trimToEmpty(account);
		email = RegularExpression.processForLogin(email);
		email = email.toLowerCase();
		return email;
	}

}
