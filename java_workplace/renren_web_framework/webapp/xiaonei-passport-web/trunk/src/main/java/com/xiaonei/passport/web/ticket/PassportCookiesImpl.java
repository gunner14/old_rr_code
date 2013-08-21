package com.xiaonei.passport.web.ticket;

import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.platform.core.opt.base.CookieManager;

@Service
public class PassportCookiesImpl implements PassportCookies {

	@Override
	public void clearAutoLoginCookie(HttpServletResponse response) {
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.EMAIL,
				0, "/",Globals.RENREN_DOMAIN);
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.PASSWORD, 0, "/",Globals.RENREN_DOMAIN);
		// 清理登录email自动填入
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.DE, 0,
				"/",Globals.RENREN_DOMAIN);
		// 清理webpager用的那个
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.ID, 0,
				"/",Globals.RENREN_DOMAIN);
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.XNSID,
				0, "/",Globals.RENREN_DOMAIN);
		CookieManager.getInstance().clearCookie(response, LoginCookieDef.KL, 0,
				"/",Globals.RENREN_DOMAIN);
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.loginKey_old_ticket, 0, "/",Globals.RENREN_DOMAIN);
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.AUTOLOGIN, 0, "/",Globals.PASSPORT_DOMAIN_OLD);
		//login.renren.com下将不再有ap
		//CookieManager.getInstance().clearCookie(response,LoginCookieDef.AUTOLOGIN, 0, "/",Globals.PASSPORT_DOMAIN);
//dont delete COOKIE_PASSPORT,because it has been delete in callback 
//		CookieManager.getInstance().clearCookie(response,
//				LoginCookieDef.COOKIE_PASSPORT, 0, "/",Globals.RENREN_DOMAIN);
	}
	
	public void clearTicket(HttpServletResponse response){
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.loginKey_ticket, 0, "/",Globals.RENREN_DOMAIN);
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.loginKey_ticket_old, 0, "/",Globals.RENREN_DOMAIN);
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.COOKIE_PASSPORT, 0, "/", Globals.RENREN_DOMAIN);
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.COOKIE_PASSPORT_OLD, 0, "/",Globals.RENREN_DOMAIN);
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.loginKey_ticket_old, 0, "/xtalk/", Globals.RENREN_DOMAIN);
		 CookieManager.getInstance().clearCookie(response, LoginCookieDef.loginKey_ticket, 0, "/xtalk/",Globals.RENREN_DOMAIN);
	}
}
