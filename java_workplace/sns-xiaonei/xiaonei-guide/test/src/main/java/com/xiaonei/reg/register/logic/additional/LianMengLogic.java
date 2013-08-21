package com.xiaonei.reg.register.logic.additional;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

public class LianMengLogic {
	private static Map<String, String> lianmengMap = new HashMap<String, String>();
	static {
		lianmengMap.put("17001", "");
		lianmengMap.put("205207", "");
	}

	public static boolean isLianMengLink(HttpServletRequest request) {
		if (StringUtils.isEmpty(request.getParameter("g"))) {
			return false;
		}
		if (StringUtils.isEmpty(request.getParameter("b"))) {
			// return false;
		}
		return true;
	}

	public static boolean isHaoyeLink(HttpServletRequest request) {
		if (lianmengMap.get(request.getParameter("ss")) == null) {
			return false;
		}
		return isLianMengLink(request);
	}
	public static boolean isVideoLink(HttpServletRequest request) {
		if (lianmengMap.get(request.getParameter("action_id")) == null) {
			return false;
		}
		return isLianMengLink(request);
	}
}
