package com.xiaonei.passport.web.passport;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;

public interface PassportLogin {
	

	/**
	 * login<br>
	 * 
	 * @param request
	 *            {@link HttpServletRequest}
	 * @param response
	 *            {@link HttpServletResponse}
	 * @param isAutoLogin
	 *            {@link Boolean}
	 * @param email
	 *            {@link String}
	 * @param pwd
	 *            {@link String}
	 * @param domain
	 *            {@link String}
	 * @param chains
	 *            {@link LoginResultChains}
	 * @return {@link LoginStatusCode}
	 * 
	 * @author tai.wang@opi-corp.com Jan 14, 2010 - 10:54:04 AM
	 */
	public LoginResult login(String userName, String md5Password,
			boolean isAutoLogin, String origUrl, String icode,
			String captchaType, String ick, String domain, long expirePeriod,
			int loginRegistrationId, boolean createRenRenTicket,
			HttpServletRequest request) ;
	
	public LoginResult login(PassportForm pForm ,boolean createRenRenTicket,
			HttpServletRequest request);




}
