package com.xiaonei.login.controllers;

import java.net.URL;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.logic.ticket.PassportTicketOprImpl;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;

@Path("callback.do")
@IgnorePassportValidation
@DontPutOrigURL
public class SetTicketLoginController {

	@Autowired
	private PassportTicketOprImpl passportTicketOprImpl;

	@Get
	@Post
	public String index(
			Invocation inv, // NL
			@Param(value = "domain") @DefValue("renren.com") String domain, //
			@Param(value = "t") String ticket, //
			@Param(value = "p3p") boolean p3p, //
			@Param(value = "autoLogin") boolean autoLogin, //
			@Param(value = "needNotify") boolean needNotify,
			@Param(value = "origURL") String origUrl) {
		if (p3p) {
			inv.getResponse().setHeader("P3P", "CP=\"CAO PSA OUR\"");
		}
		try {
			passportTicketOprImpl.setTicket(ticket, autoLogin,
					inv.getRequest(), inv.getResponse());
			origUrl = origUrl == null || origUrl.trim().length() == 0
					|| StringUtils.contains(origUrl, "PLogin.do") ? OpiConstants.urlHome
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
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		CookieManager.getInstance().saveCookie(inv.getResponse(), "loginfrom",
				"syshome");
		inv.getRequest().setAttribute("loginform", "syshome");
		return "r:" + origUrl;
	}
}
