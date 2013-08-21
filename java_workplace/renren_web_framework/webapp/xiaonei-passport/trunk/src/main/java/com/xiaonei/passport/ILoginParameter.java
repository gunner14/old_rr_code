package com.xiaonei.passport;


public interface ILoginParameter {

	public String getUserName();
	
	public String getMd5Password();
	
	public boolean isAutoLogin();
	
	public String getCaptchaCode();
	
	public String getCaptchaType();
	
	public String getIck();
	
	public long getExpirePeriod();
	
	public int getLoginRegistrationId();
	
	public boolean needCreateRenRenTicket();
	
	public String getDomain();
	
	public ILoginContext getLogincontext();

}
