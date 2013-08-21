package com.xiaonei.passport.event.fail;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.email.AutoLoginEmailLogicImpl;
import com.xiaonei.passport.event.ILoginAfterLoginFail;
import com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.StringFormat;
@Service
public class UserFailLogic implements ILoginAfterLoginFail{
	@Autowired
	private AutoLoginEmailLogicImpl autoLoginEmailLogicImpl;


	@Override
	public boolean canProcessLoginStatusCode(LoginStatusCode code) {
		if(LoginStatusCode.FAILUSER==code){
			return true;
		}
		return false;
	}

	@Override
	public void process(LoginResult result, ILoginParameter loginParameter) {
		final String email = Escape.trim(loginParameter.getUserName());
		int loginCount=CaptchaAdapterFactory.getCaptchaAdapterInstance().getLoginCountFromContext(loginParameter.getLogincontext());
		if (loginCount < 5 || !StringFormat.isValidEmailAddr(email)
				|| StringUtils.endsWith(email, "@qq.com")) {
			return ;
		}  else  {		
			result.setSendMail(true);
			autoLoginEmailLogicImpl.sendAutoLoginEmail(email, loginParameter.getDomain());
		}
	}
}
