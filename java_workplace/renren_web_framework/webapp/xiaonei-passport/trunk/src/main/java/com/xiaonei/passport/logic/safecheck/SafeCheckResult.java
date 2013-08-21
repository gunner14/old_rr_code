package com.xiaonei.passport.logic.safecheck;


import java.util.HashMap;
import java.util.Map;

import com.xiaonei.aegis.login.client.CheckLoginClient;
import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.logger.LoginLoggerDef;

public class SafeCheckResult {
	private static SafeCheckResult _instance=new SafeCheckResult();
	private SafeCheckResult(){}
	public static SafeCheckResult getInstance(){
		return _instance;
	}
	
	/**
	 * safeCheckLogin<br>
	 * 安全登录检查
	 * 
	 * @param request
	 * @param userName
	 * @param password
	 * @param ip
	 * @param account
	 * 
	 * @author tai.wang@opi-corp.com Jan 14, 2010 - 10:44:51 AM
	 */
	public void safeCheckLogin(int userId, int sourceType, ILoginParameter loginParameter, ILoginContext context) {
		if (!LoginLoggerDef.loginSafe.isEnable()) {
			return;
		}
		try {
			Map<String, String> map=new HashMap<String, String>();
			map.put("autoLogin", String.valueOf(loginParameter.isAutoLogin()));
			map.put("Host", context.getHost());
			//map.put("X-Forwarded-For", context.getxForwardedFor().nextElement());
			map.put("User-Agent", context.getUserAgent());
			map.put("Referer", context.getReferer());
			String userName=loginParameter.getUserName();
			String md5Password=loginParameter.getMd5Password();
			CheckLoginClient.getInstance().loginCheckTestWithMap(
					userId, userName,
					md5Password, context.getIp(),
					true,
					System.currentTimeMillis(),sourceType, map);
					
		} catch (final Exception e) {
			LoginLoggerDef.loginSafe.error("", e);
		}
	}
}
