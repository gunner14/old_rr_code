package com.dodoyo.login.logic;


public class LogRegLogic {

	private static LogRegLogic _instance = new LogRegLogic();

	public static LogRegLogic getInstance() {
		return _instance;
	}

	private LogRegLogic() {
	}
	
	public boolean checkEmailDomain(String email){
		if (email == null || email.length() == 0) {
			return false;
		}
		String[] emailArr = email.split("@");
		if (emailArr == null || emailArr.length <2 )
			return false ;
		String domain = emailArr[1].toLowerCase();
		if("hotmail.com".equals(domain) || "163.com".equals(domain)
				|| "126.com".equals(domain) || "sina.com".equals(domain)
				|| "yahoo.com.cn".equals(domain) || "qq.com".equals(domain)
				|| "sohu.com".equals(domain) || "gmail.com".equals(domain)
				|| "yahoo.cn".equals(domain) || "msn.com".equals(domain)
				|| "live.cn".equals(domain) || "tom.com".equals(domain)
				|| "yahoo.com".equals(domain) || "sina.com.cn".equals(domain)
				|| "21cn.com".equals(domain) || "yeah.net".equals(domain)
				/* || "xnmsn.cn".equals(domain) */ || "139.com".equals(domain)
				|| "xnmsn.com".equals(domain) || "vip.qq.com".equals(domain)
				){
			return true;
		}
		return false;
	}
}
