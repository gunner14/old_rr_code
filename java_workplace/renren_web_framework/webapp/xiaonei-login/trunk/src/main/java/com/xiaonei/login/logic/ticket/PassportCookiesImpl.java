package com.xiaonei.login.logic.ticket;

import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.platform.core.opt.base.CookieManager;

@Service
public class PassportCookiesImpl implements PassportCookies {

	@Override
	public void clearAutoLoginCookie(HttpServletResponse response) {
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.EMAIL,
				0, "/");
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.PASSWORD, 0, "/");
		// 清理登录email自动填入
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.DE, 0,
				"/");
		// 清理webpager用的那个
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.ID, 0,
				"/");
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.XNSID,
				0, "/");
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.KL, 0,
				"/");
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.loginKey_old_ticket, 0, "/");
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.AUTOLOGIN, 0, "/");
//dont delete COOKIE_PASSPORT,because it has been delete in callback 
//		CookieManager.getInstance().clearCookie(response,
//				LoginCookieDef.COOKIE_PASSPORT, 0, "/");
	}
}
