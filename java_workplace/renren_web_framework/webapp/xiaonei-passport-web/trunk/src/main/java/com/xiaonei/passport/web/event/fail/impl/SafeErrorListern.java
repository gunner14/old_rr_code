package com.xiaonei.passport.web.event.fail.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginResponse;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.event.fail.IFailLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
@Service
public class SafeErrorListern implements IFailLoginListern {

	@Override
	public String process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult result) {
		return "http://safe.renren.com/home";
	}

	@Override
	public boolean support(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult resultWrapper) {
		if(LoginStatusCode.FAILNOSAFEUSER.getCode()==code){
			return true;
		}
		if(resultWrapper != null){
			LoginResponse result=resultWrapper.getLoginResponse();
			if(result!=null&&result.isGotoSafeCenter()){
				return true;
			}
		}
		return false;
	}

}
