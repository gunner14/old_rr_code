package com.xiaonei.passport;
import java.util.Map;


public interface ILoginContext extends ILoginLocalVar{
	
	public String getHost();
	
	public String getUserAgent();
	
	public String getReferer() ;
	
	//public Enumeration<String> getxForwardedFor();
	
	public String getIp();
	
	public Map<String, String> getCooikes() ;
	
	public String getRequestUrl();
	
}
