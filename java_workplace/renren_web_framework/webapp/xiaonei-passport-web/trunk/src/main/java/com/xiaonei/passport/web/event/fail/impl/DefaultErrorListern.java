package com.xiaonei.passport.web.event.fail.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.OrigUrlDefaultDef;
import com.xiaonei.passport.web.event.fail.IFailLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.OrigUrlUtil;
@Service
public class DefaultErrorListern implements IFailLoginListern {

	@Override
	public String process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult resultWrapper) {
		
	    String errorUrl = LoginContextUtil.getErrorUrl(pForm.getErrorUrl(),OrigUrlDefaultDef.DEFAULT_ORIG_URL);
		
		int count = 0;
        final LoginResult result=LoginResult.getDefualLoginResultWrapper();
        if(result != null && result.getLoginResponse()!=null){
            count=result.getLoginResponse().getCatchaCount();
        }   
       
        return OrigUrlUtil.getUrl(count, errorUrl, pForm.getOrigURL(), code, pForm.getEmail(), pForm.getLoginRegistrationId(), null);
	}
	@Override
	public boolean support(HttpServletRequest request, HttpServletResponse response, PassportForm pForm, int code, LoginResult result) {
		if(LoginStatusCode.LOGIN_OK.getCode() == code){
			return false;
		}
		return true;
	}
   
}
