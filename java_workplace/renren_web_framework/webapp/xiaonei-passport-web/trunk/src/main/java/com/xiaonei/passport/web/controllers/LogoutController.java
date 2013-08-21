package com.xiaonei.passport.web.controllers;

import java.net.URL;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.renren.passport.service.ILoginService;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.passport.ticket.TicketServiceFactory;
import com.xiaonei.passport.util.StatUtil;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.ticket.PassportCookiesImpl;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.UrlReturnUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;

@Controller
public class LogoutController {
	@Autowired
	private PassportCookiesImpl passportCookiesImpl;
	
	private ILoginService loginService=PassportAdaptFactory.getLoginService();

	private ITicketService ticketService = TicketServiceFactory.getInstance();
	
	@RequestMapping(value = { "/logout.do", "/Logout.do" }, method = RequestMethod.GET)
	public String index(
			final HttpServletRequest request,
			HttpServletResponse response,
			@RequestParam(value = "get_check", required = false) final String getCheck,
			@RequestParam(value = "origURL", required = false) String origURL) {
		final StatUtil su = new StatUtil();
//		如果验证csrf攻击 会出错
        
		String checkRt;
		if ((checkRt = this.checkCsrfRt(request, getCheck)) != null) {
			String utf8url=LoginContextUtil.getOrigUrlEncode(origURL);
			String url = StringUtils.isNotEmpty(origURL) ? checkRt
					+ "&origURL=" + utf8url : checkRt;
			UrlReturnUtil.redirectUrl(response, url);
			return null;
		}
		
		checkOrigURL(origURL);
		
        
		LoginLoggerDef.logout.trace("origURL:" + origURL);
		final String ticket = CookieManager.getInstance().getCookie(request,
				LoginCookieDef.loginKey_ticket);
		final String passportTicket = CookieManager.getInstance().getCookie(request,
				LoginCookieDef.COOKIE_PASSPORT);
		
		loginService.logout(passportTicket, ticket, LoginContextUtil.getLoginContext(request));
//		BaseThreadUtil.clearHost(request);
		this.clearAutoLoginCookie(response);
		LoginLoggerDef.logout.trace("clearAutoLoginCookie");
		clearTicketCookie(response);
		LoginLoggerDef.logout.trace("clearTicketCookie");
		su.stepTimeCost("1:LogoutAction:CleanTicket");
		su.getWholeTime("LogoutSuccFinish");
		if (origURL == null || origURL.trim().length() == 0) {
			origURL = "http://www.renren.com/SysHome.do";
		}else{
			origURL=LoginContextUtil.getOrigUrl(origURL);
		}
		UrlReturnUtil.redirectUrl(response, origURL);
		return null;

	}

	/**
	 * checkCsrfRt<br>
	 * 
	 * @param inv
	 * @param getCheck
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Mar 8, 2010 - 8:41:31 PM
	 */
	private String checkCsrfRt(final HttpServletRequest request,
			final String getCheck) {
		final String ticket = CookieManager.getInstance().getCookie(request,
				LoginCookieDef.loginKey_ticket);
		if (StringUtils.isEmpty(ticket)) {
			return null;
		}
		if ((getCheck != null) && (getCheck.trim().length() > 0)) {
			String code = "";
			LoginLoggerDef.logout.trace("ticket:" + ticket);
			if (ticket != null) {
				code += ticket.hashCode();
				if (getCheck.equals(code)) {
					return null;
				} else {
					return "http://login.renren.com/logout.do?get_check="
							+ ticket.hashCode();
				}

			}
		} else {
			return "http://login.renren.com/logout.do?get_check="
					+ ticket.hashCode();
		}
		return null;
	}

	private void clearAutoLoginCookie(final HttpServletResponse response) {
		this.passportCookiesImpl.clearAutoLoginCookie(response);
	}

	private void clearTicketCookie(final HttpServletResponse response) {
		this.passportCookiesImpl.clearTicket(response);
	}
	
	private void checkOrigURL(String origURL) {
	    if (StringUtils.isNotEmpty(origURL)) {
            try {
                origURL = URLEncoder.encode(origURL, "UTF-8");
            } catch (final Exception e) {
                e.printStackTrace();
            }
        }
        
        //避免跳到站外
        try {
            URL callBackUrl = new URL(origURL);
            if(callBackUrl.getHost().indexOf(OpiConstants.getCurrentDomain()) == -1){
                if(! callBackUrl.getHost().equals("56.com")){
                    origURL = OpiConstants.urlMain.toString();
                }
            }   
        } catch (Exception e) {
            
        }
	}
	    
}
