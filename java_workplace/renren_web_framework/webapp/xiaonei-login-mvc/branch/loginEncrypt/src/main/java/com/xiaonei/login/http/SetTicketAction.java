package com.xiaonei.login.http;

import java.net.URL;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.xce.passport.PassportAdapterFactory;

public class SetTicketAction extends Action {

	@Override
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		StatUtil su = new StatUtil();
		String ticket = request.getParameter("t");
		if (request.getParameter("p3p") != null) {
			response.setHeader("P3P", "CP=\"CAO PSA OUR\"");
		}
		int hostid = 0;
		try {
			if (ticket != null && ticket.trim().length() > 0) {
				if (Boolean.valueOf(request.getParameter("autoLogin"))) {
					// 无条件写入ticket
					CookieManager.getInstance().saveCookie(response,
							PassportManager.loginKey_ticket, ticket, 6 * 3600,
							"/");
					CookieManager.getInstance().saveCookie(response,
							PassportManager.loginKey_old_ticket, ticket,
							6 * 3600, "/");
				} else {
					// 无条件写入ticket
					CookieManager.getInstance().saveCookie(response,
							PassportManager.loginKey_ticket, ticket);
					CookieManager.getInstance().saveCookie(response,
							PassportManager.loginKey_old_ticket, ticket);
				}
				// Webpager用
				hostid = PassportAdapterFactory.getWebTicket().verifyTicket(
						ticket,
						PassportManager.getInstance()
								.getTicketProfiler(request));
				su.stepTimeCost("1:CallbackAction:verifyTicket");
				// 他们要的(webpager JS FE)
				CookieManager.getInstance().saveCookie(response, "id",
						"" + hostid);
				// 黄欢要的
				CookieManager.getInstance()
						.saveCookie(
								response,
								"xnsid",
								Integer.toHexString((int) (System
										.currentTimeMillis() ^ -1)));
			}
		} catch (Exception e) {
		}
		String origUrl = request.getParameter("origURL");
		origUrl = origUrl == null || origUrl.trim().length() == 0 ? OpiConstants.urlHome
				+ "/Home.do"
				: origUrl;
		
		//避免跳到站外
		try {
			URL callBackUrl = new URL(origUrl);
			if(callBackUrl.getHost().indexOf(OpiConstants.getCurrentDomain()) == -1){
				origUrl = OpiConstants.urlMain.toString();
			}	
		} catch (Exception e) {
			
		}
		

		// 交叉登录的逻辑部分(不用时就删了)
		if (hostid > 0) {
			CookieManager.getInstance().saveCookie(response, "kl",
					"kl_" + hostid);
			if (Boolean.parseBoolean(request.getParameter("needNotify"))
					&& !PassportAdapterFactory.getWebLogin().getStatus(hostid)) {
				// 是一个交叉登录
				try {
					su.getWholeTime("CallbackWithNotifySuccLogin");
					return new ActionForward(OpiConstants.urlLogin
							+ "/LN.do?origURL="
							+ URLEncoder.encode(origUrl, "UTF-8"), true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			su.stepTimeCost("2:CallbackAction:getNotifyStatus");
		}
		su.getWholeTime("CallbackSuccLogin");
		return new ActionForward(origUrl, true);
	}
}
