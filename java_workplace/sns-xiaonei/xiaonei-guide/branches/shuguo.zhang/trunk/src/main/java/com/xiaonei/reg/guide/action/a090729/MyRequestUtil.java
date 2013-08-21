package com.xiaonei.reg.guide.action.a090729;

import javax.servlet.http.HttpServletRequest;

public class MyRequestUtil {
	public static String getBrowserInfo(HttpServletRequest request){
		String info = null;
		info = request.getHeader("User-Agent");
		if(info == null) return " UA[no agent]";
		return " UA["+info+"]";
	}
}
