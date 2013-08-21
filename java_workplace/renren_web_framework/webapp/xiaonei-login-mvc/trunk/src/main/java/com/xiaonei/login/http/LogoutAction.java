package com.xiaonei.login.http;

import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;

public class LogoutAction extends Action {

	@Override
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		StatUtil su = new StatUtil();
		clearAutoLoginCookie(response);
		String origURL = request.getParameter("origURL");
		if (origURL != null && origURL.trim().length() > 0) {
			try {
				origURL = URLEncoder.encode(origURL, "UTF-8");
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		String getCheck = null;
		if ((getCheck = request.getParameter("get_check")) != null) {
			String ticket, code = "";
			ticket = CookieManager.getInstance().getCookie(request,
					PassportManager.loginKey_ticket);
			if (ticket != null) {
				code += ticket.hashCode();
				if (getCheck.equals(code)) {
					Logger.getLogger("Logout").info("Logout Succ!!!");
				} else {
					Logger.getLogger("Logout")
							.error(
									"Logout CSRF Error:"
											+ request.getHeader("Referer"));
				}

			}
		} else {
			Logger.getLogger("Logout").error(
					"Logout Url Error:" + request.getHeader("Referer"));
		}

		PassportManager.getInstance().clearHostId(request, response);
		su.stepTimeCost("1:LogoutAction:CleanTicket");

		su.getWholeTime("LogoutSuccFinish");
		return new ActionForward(OpiConstants.urlPassport + "/relogin.do" // NL
				+ "?domain=" + OpiConstants.getCurrentDomain() // NL
				+ (origURL != null ? "&origURL=" + origURL : "") // NL
		, true);
	}

	private void clearAutoLoginCookie(HttpServletResponse response) {
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
		CookieManager.getInstance().clearCookie(response,
				LoginCookieDef.COOKIE_PASSPORT, 0, "/");
	}
}
