package com.xiaonei.passport.web.event.before.impl;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.event.before.IBeforeLoginListern;
import com.xiaonei.passport.web.model.PassportForm;
@Service
public class GetRequestPassportDomainValidation implements IBeforeLoginListern{

	@Override
	public LoginStatusCode process(HttpServletRequest request, HttpServletResponse response, PassportForm pForm) {
		if(!StringUtils.endsWith(request.getServerName(),Globals.PASSPORT_DOMAIN)
				&& !StringUtils.endsWith(request.getServerName(), Globals.PASSPORT_DOMAIN_OLD)){
			return  LoginStatusCode.FAILNOTPASSPORTDOMAIN;
		}
		return LoginStatusCode.LOGIN_OK;
	}


}
