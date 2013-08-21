package com.xiaonei.passport.util;

import java.util.HashMap;

import org.springframework.stereotype.Service;

public class PassportEmailUtils{

	public static String emailLoginUrl(String email) {
		if (email == null || email.length() == 0) {
			return "";
		}

		String emailPortal = null;
		HashMap<String, String> emailPortals = new HashMap<String, String>();
		emailPortals.put("163.com", "http://mail.163.com");
		emailPortals.put("126.com", "http://mail.126.com");
		emailPortals.put("qq.com", "http://mail.qq.com");
		emailPortals.put("yahoo.com.cn", "http://cn.mail.yahoo.com");
		emailPortals.put("sina.com", "http://mail.sina.com.cn");
		emailPortals.put("hotmail.com", "http://mail.live.com");
		emailPortals.put("yahoo.com", "http://mail.yahoo.com");
		emailPortals.put("tom.com", "http://mail.tom.com");
		emailPortals.put("yahoo.cn", "http://cn.mail.yahoo.com");
		emailPortals.put("gmail.com", "http://mail.google.com");
		emailPortals.put("sohu.com", "http://mail.sohu.com");
		emailPortals.put("eyou.com", "http://freemail.eyou.com");
		emailPortals.put("21cn.com", "http://passport.21cn.com");
		emailPortals.put("sina.com.cn", "http://mail.sina.com.cn");
		emailPortals.put("163.com.cn", "http://163.com.cn");
		emailPortals.put("yeah.net", "http://yeah.net");
		emailPortals.put("msn.com", "http://mail.live.com");
		emailPortals.put("msn.cn", "http://cn.msn.com");
		emailPortals.put("126.com.cn",
				"http://126.com.cn/search.html?q=%D3%CA%CF%E4");
		emailPortals.put("ncu.edu.cn", "http://ncu.edu.cn");
		emailPortals.put("mail.china.com", "http://mail.china.com");
		emailPortals.put("live.cn", "http://mail.live.com");

		String[] emailArr = email.split("@");
		String domain;
		try {
			domain = emailArr[1].toLowerCase();
		} catch (Exception e) {
			return "";
		}
		emailPortal = emailPortals.get(domain);

		return ((emailPortal == null) ? "" : emailPortal);
	}
}
