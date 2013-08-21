package com.xiaonei.passport.web.event.after.impl;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.LoginResultCode;
import com.xiaonei.passport.web.event.after.IAfterLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;

@Service
public class IdReturnListern implements IAfterLoginListern{

	@Override
	public LoginStatusCode process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			LoginResult resultWrapper) {
		LoginResponse result=resultWrapper.getLoginResponse();
		resultWrapper.addResultContents(LoginResultCode.ID, result.getUserId());
		resultWrapper.addResultContents(LoginResultCode.REAL_ID, result.getRealUserId());
		return LoginStatusCode.LOGIN_OK;
	}

}
