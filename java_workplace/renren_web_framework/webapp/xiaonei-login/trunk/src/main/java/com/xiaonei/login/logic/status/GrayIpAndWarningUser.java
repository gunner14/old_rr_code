package com.xiaonei.login.logic.status;

import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginStatusCode;

public interface GrayIpAndWarningUser {

	/**
	 * getRtAddrForGrayIpOrWaringUser<br>
	 * 
	 * @param inv
	 * @param pForm
	 * 
	 * @author tai.wang@opi-corp.com Jan 25, 2010 - 7:08:07 PM
	 * @return
	 */
	public LoginStatusCode getRtAddrForGrayIpOrWaringUser(final Invocation inv,
			final String email, String pwd, String icode);
}
