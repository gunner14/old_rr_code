package com.xiaonei.passport.web.event.after.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.event.after.IAfterLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.ticket.PassportTicketOpr;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;
@Service
public class PassportSaveListern implements IAfterLoginListern {
	@Autowired
	private PassportTicketOpr passportTicketOpr;
	@Override
	public LoginStatusCode process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm, LoginResult result) {
		boolean isPersistent=false;
		if (pForm.isAutoLogin()) {
			isPersistent=true;
		}
		String ap=CookieManager.getInstance().getCookie(request, LoginCookieDef.AUTOLOGIN);
		if(!StringUtils.isEmpty(ap)){
			isPersistent=true;
		}
		saveTicket(request, response, pForm, result.getLoginResponse(), isPersistent);
		return LoginStatusCode.LOGIN_OK;
	}

	/**
	 * set TICKET
	 * @param request
	 * @param response
	 * @param pForm
	 * @param result
	 * @param isPersistent
	 */
	private void saveTicket(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm, LoginResponse result, boolean isPersistent){
		passportTicketOpr.savePassport2Cookie(result.getUserId(), request, response, result.getPassportTicket(), result.getOldPassport(), isPersistent);
	}
}
