package com.xiaonei.passport.web.controllers;

import java.net.URL;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.LoginType;
import com.xiaonei.passport.constants.Globals;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Controller
public class CallbackSetTicketController {
	
	private ILoginService webLoginService=PassportAdaptFactory.getLoginService();
	
	protected Log logger = LogFactory.getLog(this.getClass());
	
	@RequestMapping(value = { "/callback.do", "/callback.do" }, method=RequestMethod.GET)
	public String index(final HttpServletRequest request, final HttpServletResponse response,
			@RequestParam(value = "t", required = false) String ticket,
			@RequestParam(value = "origURL", required = false) String origUrl) {
		/*if (p3p) {
			response.setHeader("P3P", "CP=\"CAO PSA OUR\"");
		}*/
		
		//只能根据cookie里是否有passport和ticket来判断走的是新登陆还是老登陆
		String ntTicket = CookieManager.getInstance().getCookie(request, LoginCookieDef.loginKey_ticket);
		int hostId = 0;
		if(! StringUtils.isEmpty(ntTicket)){
			Long userId = webLoginService.verifyTicket(ntTicket, LoginType.Web);
			hostId = userId.intValue();
            // 无条件写入ticket
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
            		ntTicket, -1, "/", "." + Globals.RENREN_DOMAIN);
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
                    Encoder.encode(ntTicket), -1, "/xtalk/", "."+Globals.RENREN_DOMAIN);
			
		} 
		
		/**因为这里的redirect要到www.renren.com下面，所以需要兼容老的票*/
		if(! StringUtils.isEmpty(ticket)){
			hostId = PassportAdapterFactory.getWebTicket().verifyTicket(ticket,
	                 PassportManager.getInstance().getTicketProfiler(request));
			// 无条件写入ticket
	        CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket_old,
	                 ticket, -1, "/", "." + OpiConstants.domainMain);
	        CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket_old,
	                 Encoder.encode(ticket), -1, "/xtalk/", OpiConstants.domainMain.toString());
	        CookieManager.getInstance().saveCookie(response, "societyguester", ticket, -1, "/",
	                 "." + OpiConstants.domainMain);
		}
		 
		// 他们要的(webpager JS FE)
        CookieManager.getInstance().saveCookie(response, LoginCookieDef.ID, "" + hostId);
        // 黄欢要的
        CookieManager.getInstance().saveCookie(response, LoginCookieDef.XNSID,
                 Integer.toHexString((int) (System.currentTimeMillis() ^ -1)));
        
        origUrl = origUrl == null || origUrl.trim().length() == 0 || StringUtils.contains(origUrl, "PLogin.do") 
        		? OpiConstants.urlHome + "/Home.do" : origUrl;
        
        //避免跳到站外
		try {
			URL callBackUrl = new URL(origUrl);
			if(callBackUrl.getHost().indexOf(OpiConstants.getCurrentDomain()) == -1){
				origUrl = OpiConstants.urlMain.toString();
			}	
		} catch (Exception e) {
			
		}
		
		CookieManager.getInstance().saveCookie(response, "loginfrom", "syshome");
		request.setAttribute("loginform", "syshome");
		try {
			response.sendRedirect(origUrl);	
		} catch (Exception e) {
			logger.error("sendRedirect error, origUrl is: "+origUrl , e);
		}
		return null;
	}
}
