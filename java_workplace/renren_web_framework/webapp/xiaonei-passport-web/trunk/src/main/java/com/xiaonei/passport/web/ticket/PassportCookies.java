package com.xiaonei.passport.web.ticket;

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
	
	/**
	 * 清除t和p票，由于兼容需要清除老的p和t票
	 * @param response
	 */
	public void clearTicket(HttpServletResponse response);
}
