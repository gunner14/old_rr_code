package com.xiaonei.passport.web.event.before.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.event.before.IBeforeLoginListern;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.utility.DES_Encrypt;
@Service
public class UserNameSaveInCookie implements IBeforeLoginListern{

	@Override
	public LoginStatusCode process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm) {
		// 记住用户名，用于下次在输入框自动显示
        CookieManager.getInstance().saveCookie(response, LoginCookieDef.DE,
                DES_Encrypt.encrypt(pForm.getEmail()), 60 * 60 * 24 * 30 * 12,// one year
        "/");
        return LoginStatusCode.LOGIN_OK;
	}

}
