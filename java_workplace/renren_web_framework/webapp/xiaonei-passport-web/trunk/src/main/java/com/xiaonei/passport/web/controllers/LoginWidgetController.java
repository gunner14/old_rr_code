package com.xiaonei.passport.web.controllers;

import java.net.URL;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.web.adapt.LoginConfigureImpl;
import com.xiaonei.websecurity.adapter.SafeFilter;

@Controller
public class LoginWidgetController {
	protected Log logger = LogFactory.getLog(this.getClass());
	
	@RequestMapping(value = { "/widget.do" }, method = RequestMethod.GET)
	public String getBranch(final HttpServletRequest request, final HttpServletResponse response) {
		String userAgent = request.getHeader("User-Agent");
		String forBiddenUAs = LoginConfigureImpl.getLocalInstance().getUniqLoginConfigure(0, LoginConfigureKey.FORBIDDEN_UA.getKey());
		boolean isCurrentBrowserForbiddenUseHttps = false;
		if(!StringUtils.isEmpty(forBiddenUAs)){
			String[] list = forBiddenUAs.split(";");
			for(String uaItem : list){
				if(! StringUtils.isEmpty(uaItem)){
					if(userAgent.contains(uaItem)){
						isCurrentBrowserForbiddenUseHttps = true;
						break;
					}
				}
			}
		}
		request.setAttribute("forbiddenHttps", isCurrentBrowserForbiddenUseHttps);
		String loginRegistrationId = request.getParameter("loginRegistrationId");
		request.setAttribute("loginRegistrationId", loginRegistrationId);
		
		String origURL = request.getParameter("origURL");
		request.setAttribute("origURL", origURL);
		
		List<String> validatinStaticUrl=LoginConfigureImpl.getLocalInstance().getLoginConfigure(0, LoginConfigureKey.VALIDATION_STATIC_URL.getKey());
		String callBackJsUrl = request.getParameter("callBackJsUrl");
		callBackJsUrl = getUrl(callBackJsUrl, true, validatinStaticUrl);
		if(callBackJsUrl!=null){
			request.setAttribute("callBackJsUrl", callBackJsUrl);
		}
		String callBackCssUrl = request.getParameter("callBackCssUrl");
		callBackCssUrl = getUrl(callBackCssUrl, false, validatinStaticUrl);
		if(callBackCssUrl!=null){
			request.setAttribute("callBackCssUrl", callBackCssUrl);
		}
		
		return "/widget/branches.jsp";
	}
	
	@RequestMapping(value = { "/Widget.do" }, method = RequestMethod.GET)
	public String get(final HttpServletRequest request,
			final HttpServletResponse response) {
		String loginRegistration = request.getParameter("loginRegistrationId");
		int loginRegistrationId = LoginConfigureImpl.DEFAULT_LOGIN_REGISTRATION_ID;
		if (!StringUtils.isEmpty(loginRegistration)) {
			try {
				loginRegistrationId = Integer.valueOf(loginRegistration);
			} catch (Exception e) {
				logger.error("loginRegistrationId is not a int type", e);
				loginRegistrationId = LoginConfigureImpl.DEFAULT_LOGIN_REGISTRATION_ID;
			}
		}
		List<String> validatinStaticUrl=LoginConfigureImpl.getLocalInstance().getLoginConfigure(0, LoginConfigureKey.VALIDATION_STATIC_URL.getKey());
		String callBackJsUrl = request.getParameter("callBackJsUrl");
		callBackJsUrl = getUrl(callBackJsUrl, true, validatinStaticUrl);
		if(callBackJsUrl!=null){
			request.setAttribute("callBackJsUrl", callBackJsUrl);
		}
		String callBackCssUrl = request.getParameter("callBackCssUrl");
		callBackCssUrl = getUrl(callBackCssUrl, false, validatinStaticUrl);
		if(callBackCssUrl!=null){
			request.setAttribute("callBackCssUrl", callBackCssUrl);
		}
		
		String origURL = request.getParameter("origURL");
		request.setAttribute("origURL", origURL);
		
		String useHttps = request.getParameter("useHttps");
		request.setAttribute("useHttps", useHttps);
	
		request.setAttribute("loginRegistrationId", loginRegistrationId);
		String url =null;
		try{
			 url=LoginConfigureImpl.getLocalInstance().getUniqLoginConfigure(loginRegistrationId, LoginConfigureKey.WEB_WIDGET.getKey());
		}catch(Exception e){
			e.printStackTrace();
			logger.error("can not load widget url", e);
			url="/widget/login-widget.jsp";
		}
		
		if(StringUtils.isEmpty(url)){
			url="/widget/login-widget.jsp";
		}
		return url;
	}

	/**
	 * 对url做处理.避免xss注入
	 * 
	 * @param callBackJsUrl
	 * @return
	 */
	private String getUrl(String callBackJsUrl, boolean isJs, List<String> staticUrls) {
		if(StringUtils.isEmpty(callBackJsUrl)){
			return null;
		}
		callBackJsUrl = SafeFilter.getInstance().getRichContent(callBackJsUrl);
		try {
			URL url = new URL(callBackJsUrl);
			if (!staticUrls.contains(url.getHost())) {
				return null;
			}
		} catch (Exception e) {
			logger.error("call back url is not validation, the callBackJsUrl is "+callBackJsUrl, e);
			return null;
		}
		return callBackJsUrl;
	}

}
