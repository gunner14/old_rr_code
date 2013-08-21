package com.xiaonei.passport.web.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.renren.passport.service.LoginType;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.web.event.EventProcess;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.passport.PassportLogin;
import com.xiaonei.passport.web.ticket.PassportCookies;
import com.xiaonei.passport.web.ticket.PassportTicketOpr;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.UrlReturnUtil;

@Controller
public class RedirectLoginController {

	@Autowired
	private PassportLogin passportLogin;

	protected Log logger = LogFactory.getLog(this.getClass());

	protected PpLogger remoteLogger = LoginLoggerDef.login;
	
	@Autowired
	private PassportTicketOpr passportTicketOpr;
	
	@Autowired
	private PassportCookies passportCookies;
	
	
	/**
	 * index<br>
	 * 这个一个get,做什么用的呢?<br>
	 * 换票,主要工作
	 * 这里主要是为了针对app.renren.com下的所有页面的自动登陆，
	 * 目前线上是不支持这个自动登陆的，需要前端添加get请求再做测试，这里还没有测试过
	 * 
	 * @param inv
	 * @param domain
	 * @param origURL
	 * @param p3p
	 * @param autoLogin
	 * @return
	 * 
	 * @author zhe.long 2012.7.27
	 */
	@RequestMapping(value = { "/RL.do" }, method=RequestMethod.GET)
	public String get(@RequestParam(value = "domain", required = false)String domain,
            @RequestParam(value = "origURL", required = false)String origURL,
            @RequestParam(value = "errURL", required = false)String errorURL,
            final HttpServletRequest request,
			final HttpServletResponse response,// 负责登录的主要方法
			final PassportForm pForm) {
		LoginResult result = null;
	        
		if(origURL == null) {
            origURL = "www.renren.com/SysHome.do";
        }
        pForm.setOrigURL(origURL);
        pForm.setErrorUrl(errorURL);
        if(domain == null) {
            domain = "renren.com";
        }
        pForm.setDomain(domain);
		//下面的代码处理失败后如果跳向的url是首页，则有可能会死循环，所以必须catch任何异常，保证发生异常后一定要把票清除
		try {
			//String domain=LoginContextUtil.getDomain(pForm.getLoginRegistrationId());
			//String domain = LoginConfigureImpl.getLocalInstance().getUniqLoginConfigure(pForm.getLoginRegistrationId(), LoginConfigureKey.DOMAIN);
			//pForm.setDomain(domain);
			LoginStatusCode code = EventProcess.getInstance().fireBeforeLoginEvent(request, response, pForm, LoginConfigureKey.GET_REDIRECT_LOGIN_BEFORE_LOGIN_EVENT.getKey());
			if (LoginStatusCode.LOGIN_OK != code) {
				String redirectUrl=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, code.getCode(), LoginResult.getDefualLoginResultWrapper(),LoginConfigureKey.REDIRECT_LOGIN_FAIL_LOGIN_EVENT);
				return UrlReturnUtil.redirectUrl(response, redirectUrl);
			}
			// 用户id,判断是否来换票
			result = this.passportTicketOpr.getUserIdByPassport(
					request, response, pForm, LoginType.Web);
		} catch (Exception e) {
			// TODO: handle exception
			if(logger.isWarnEnabled()) {
				logger.warn("Before RL.do transfer method exception " + e);
			}
		}
		
		// 不是换票
		if (result==null||LoginStatusCode.LOGIN_OK.getCode()!= result.getLoginStatusCode()) {
			//passportCookies.clearTicket(response);这个时候不清除ap票的话，会千万死循环
			passportCookies.clearAutoLoginCookie(response);
			if (StringUtils.isNotEmpty(pForm.getErrorUrl())) {
				String url=LoginContextUtil.getOrigUrl( pForm.getErrorUrl());
				return UrlReturnUtil.redirectUrl(response, url);
			}
			String redirectUrl= EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, LoginStatusCode.FAILNOPOST.getCode(), null,LoginConfigureKey.REDIRECT_LOGIN_FAIL_LOGIN_EVENT);
			return  UrlReturnUtil.redirectUrl(response,redirectUrl);
		}
		EventProcess.getInstance().fireLoginSuccessEvent(request, response, pForm, result, LoginConfigureKey.WEB_GET_LOGIN_SUCCESS_LOGIN_EVENT);
		
		String url= LoginContextUtil.getOrigUrl(pForm.getOrigURL());
		return  UrlReturnUtil.getUrlReturn(request, response, url);
	}
	
	/**
	 * post<br>
	 * 负责登录的方法
	 * 负责处理www.renren.com/RL.do的POST方法
	 * 主要是app应用登陆
	 * @param inv
	 * @param pForm
	 * @return
	 * 
	 * @author zhe.long      2012.7.26
	 */
	@RequestMapping(value = { "/RL.do"}, method=RequestMethod.POST)
	public String post(@RequestParam(value = "domain", required = false)String domain,
            @RequestParam(value = "origURL", required = false)String origURL,
            @RequestParam(value = "errURL", required = false)String errorURL,
            final HttpServletRequest request,
			final HttpServletResponse response,//
			final PassportForm pForm) {         
	    if(origURL == null) {
            origURL = "www.renren.com/SysHome.do";
        }
        pForm.setOrigURL(origURL);
        pForm.setErrorUrl(errorURL);
        if(domain == null) {
            domain = "renren.com";
        }
        pForm.setDomain(domain);
		//String domain = LoginConfigureImpl.getLocalInstance().getUniqLoginConfigure(pForm.getLoginRegistrationId(), LoginConfigureKey.DOMAIN);
		pForm.setDomain(domain);
		LoginStatusCode code = EventProcess.getInstance().fireBeforeLoginEvent(request, response, pForm, LoginConfigureKey.REDIRECT_LOGIN_BEFORE_LOGIN_EVENT.getKey());
		if (LoginStatusCode.LOGIN_OK != code) {
			String redirectUrl=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, code.getCode(), LoginResult.getDefualLoginResultWrapper(),LoginConfigureKey.REDIRECT_LOGIN_FAIL_LOGIN_EVENT);
		    return UrlReturnUtil.redirectUrl(response, redirectUrl);
		}
		this.logger.debug("login posted datas are: " + pForm.toString());

		// 登录的主体
		final LoginResult result = passportLogin
				.login(pForm, true, request);
		// 登陆后
		if (result.getLoginStatusCode() == LoginStatusCode.LOGIN_OK.getCode()) {
			EventProcess.getInstance().fireLoginSuccessEvent(request, response, pForm, result, LoginConfigureKey.WEB_LOGIN_SUCCESS_LOGIN_EVENT);
			String url= LoginContextUtil.getOrigUrl( pForm.getOrigURL());
			return UrlReturnUtil.redirectUrl(response, url);
		} else {
			String redirectUrl=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, result.getLoginStatusCode(),
					result,LoginConfigureKey.REDIRECT_LOGIN_FAIL_LOGIN_EVENT);
			return UrlReturnUtil.redirectUrl(response, redirectUrl);
		}   
	}
    
}
