package com.xiaonei.passport.event.after;


import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicAfterLogin;
import com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.passport.logic.captcha.ICaptchaAdapter;
import com.xiaonei.passport.model.LoginResult;
@Service
public class CaptchaAfterLogin implements ILogicAfterLogin{
	private ICaptchaAdapter captchaAdapter=CaptchaAdapterFactory.getCaptchaAdapterInstance();
	

	@Override
	public LoginStatusCode process(LoginResult result, ILoginParameter loginParameter) {
		if(LoginStatusCode.LOGIN_OK.getCode()==result.getLoginStatusCode()){
			ILoginContext context=loginParameter.getLogincontext();
			String userName=loginParameter.getUserName();
			String md5Password=loginParameter.getMd5Password();
			int userId=result.getUserId();
			String ip=loginParameter.getLogincontext().getIp();
			captchaAdapter.processLoginCountAfterLoginSuccess(context, userName, md5Password, userId, ip);
		}
		return LoginStatusCode.LOGIN_OK;
	}

}
