package com.xiaonei.login.logic.ticket;

import javax.servlet.http.HttpServletResponse;

public interface PassportCookies {

	/**
	 * clearAutoLoginCookie<br>
	 * 清除cookie
	 * 
	 * @param response
	 * 
	 * @author tai.wang@opi-corp.com Dec 28, 2009 - 12:29:40 PM
	 */
	public void clearAutoLoginCookie(HttpServletResponse response);
}
