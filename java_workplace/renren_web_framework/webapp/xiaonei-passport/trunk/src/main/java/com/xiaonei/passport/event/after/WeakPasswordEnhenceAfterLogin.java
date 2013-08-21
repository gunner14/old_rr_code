package com.xiaonei.passport.event.after;


import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicAfterLogin;
import com.xiaonei.passport.logic.captcha.IWeakPassword;
import com.xiaonei.passport.model.LoginResult;
@Service
public class WeakPasswordEnhenceAfterLogin implements ILogicAfterLogin {
	
	private IWeakPassword weakPassword= com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory.getWeakPasswordInstance();
	

	@Override
	public LoginStatusCode process(LoginResult result, ILoginParameter loginParameter) {
		int userId=result.getUserId();
		//为了测试修改，目前把validate(userId)注释掉， 不去验证弱密码
		if(userId!=-1&& validate(userId)){
		//if(userId!=-1){
			result.setWeakPassword(true);
		}
		return LoginStatusCode.LOGIN_OK;
	}

	/**
	 * 判断是否为弱密码
	 * @param userId user id
	 * @return
	 */
	private boolean validate(int userId){
		return weakPassword.isWeakPassword(userId);
	}
}
