package com.xiaonei.passport.web.event.after;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;

public interface IAfterLoginListern {
	
	public LoginStatusCode process(HttpServletRequest request, HttpServletResponse response, PassportForm pForm,  LoginResult result);
	
}
