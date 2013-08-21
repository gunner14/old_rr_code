package com.xiaonei.passport.web.event.after.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.event.after.IAfterLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.ticket.PassportTicketOprImpl;
import com.xiaonei.platform.core.opt.base.CookieManager;
@Service
public class TicketSaveListern implements IAfterLoginListern {
	 @Autowired
		private PassportTicketOprImpl passportTicketOprImpl;
		@Override
		public LoginStatusCode process(HttpServletRequest request,
				HttpServletResponse response, PassportForm pForm, LoginResult resultWrapper) {
			LoginResponse result=resultWrapper.getLoginResponse();
			passportTicketOprImpl.setTicket(result.getTicket(), result.getOldTicket(), pForm.isAutoLogin(), request, response);
			CookieManager.getInstance().saveCookie(response, "loginfrom", "syshome");
			return LoginStatusCode.LOGIN_OK;
		}

}
