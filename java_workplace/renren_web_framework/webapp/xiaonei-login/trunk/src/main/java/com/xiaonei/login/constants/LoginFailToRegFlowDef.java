package com.xiaonei.login.constants;

import java.util.HashMap;
import java.util.Map;

public class LoginFailToRegFlowDef {

	public static Map<String, Boolean> blackEmailList = new HashMap<String, Boolean>();

	static {
		blackEmailList.put("hotmail.com", true);
		blackEmailList.put("163.com", true);
		blackEmailList.put("126.com", true);
		blackEmailList.put("sina.com", true);
		blackEmailList.put("yahoo.com.cn", true);
		blackEmailList.put("qq.com", true);
		blackEmailList.put("sohu.com", true);
		blackEmailList.put("gmail.com", true);
		blackEmailList.put("yahoo.cn", true);
		blackEmailList.put("msn.com", true);
		blackEmailList.put("live.cn", true);
		blackEmailList.put("tom.com", true);
		blackEmailList.put("yahoo.com", true);
		blackEmailList.put("sina.com.cn", true);
		blackEmailList.put("21cn.com", true);
		blackEmailList.put("yeah.net", true);
		blackEmailList.put("xnmsn.cn", true);
		blackEmailList.put("139.com", true);
		blackEmailList.put("xnmsn.com", true);
		blackEmailList.put("vip.qq.com", true);
	}
}
