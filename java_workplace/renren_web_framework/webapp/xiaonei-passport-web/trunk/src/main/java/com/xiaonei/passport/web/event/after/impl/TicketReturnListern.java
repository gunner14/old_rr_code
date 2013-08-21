package com.xiaonei.passport.web.event.after.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.LoginResultCode;
import com.xiaonei.passport.web.event.after.IAfterLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
@Service
public class TicketReturnListern  implements IAfterLoginListern{
	@Override
	public LoginStatusCode process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			LoginResult resultWrapper) {
		LoginResponse result=resultWrapper.getLoginResponse();
		if(!StringUtils.isEmpty(result.getOldPassport())){
			resultWrapper.addResultContents(LoginResultCode.ELD_TICKET, result.getOldTicket());
		}
		resultWrapper.addResultContents(LoginResultCode.TICKET, result.getOldTicket());
		return LoginStatusCode.LOGIN_OK;
	}

}
