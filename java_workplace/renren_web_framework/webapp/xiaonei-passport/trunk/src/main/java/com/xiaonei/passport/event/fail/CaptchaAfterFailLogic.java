package com.xiaonei.passport.event.fail;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILoginAfterLoginFail;
import com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.passport.logic.captcha.ICaptchaAdapter;
import com.xiaonei.passport.model.LoginResult;
@Service
public class CaptchaAfterFailLogic implements ILoginAfterLoginFail{
	private ICaptchaAdapter captchaAdapter=CaptchaAdapterFactory.getCaptchaAdapterInstance();
	@Override
	public void process(LoginResult result, ILoginParameter loginParameter) {
		ILoginContext context=loginParameter.getLogincontext();
		String userName=loginParameter.getUserName();
		String md5Password=loginParameter.getMd5Password();
		captchaAdapter.processLoginCountAfterLoginFail(context, userName, md5Password, result.getCode(), context.getIp());
		result.setCatchaCount(captchaAdapter.getLoginCountFromContext(context));
	}
	@Override
	public boolean canProcessLoginStatusCode(LoginStatusCode rt){
		if ((rt != LoginStatusCode.FAILUSER)
                && (rt != LoginStatusCode.FAILVALIDATELOGIN)&&(rt!=LoginStatusCode.FAILUSERNOTEXIST)) {
            return false;
        }
		return true;
	}
}
