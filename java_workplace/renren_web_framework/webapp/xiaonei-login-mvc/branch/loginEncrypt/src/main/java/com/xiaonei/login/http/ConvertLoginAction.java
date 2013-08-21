package com.xiaonei.login.http;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;

/**
 * 已经不再使用
 * 
 * @author blackwing
 * 
 */
public class ConvertLoginAction extends Action {

	@Override
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String passport = request.getParameter("p");
		if (passport != null && passport.trim().length() > 0) {
			// 无条件写入passport
			CookieManager.getInstance().saveCookie(response,
					LoginAction.COOKIE_PASSPORT, passport, 30 * 24 * 60 * 60,
					"/", LoginAction.PASSPORT_DOMAIN);
		}
		String domain = request.getParameter(LoginAction.PARAM_DOMAIN);
		if (domain == null || domain.trim().length() <= 0) {
			domain = "renren.com";
		}
		request.setAttribute(LoginAction.PARAM_DOMAIN, domain);
		OpiConstants.setCurrentDomain(domain);
		String origUrl = request.getParameter("origURL");
		origUrl = origUrl == null || origUrl.trim().length() == 0 ? OpiConstants.urlHome
				+ "/Home.do"
				: origUrl;
		return new ActionForward(origUrl, true);
	}
}
