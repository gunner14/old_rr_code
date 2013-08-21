package com.xiaonei.passport.web.controllers;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.renren.passport.service.ILoginService;
import com.renren.passport.service.LoginResponse;
import com.renren.passport.service.LoginType;
import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.util.PassportEmailUtils;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginResultCode;
import com.xiaonei.passport.web.event.EventProcess;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.passport.PassportLogin;
import com.xiaonei.passport.web.ticket.PassportCookies;
import com.xiaonei.passport.web.ticket.PassportTicketOpr;
import com.xiaonei.passport.web.util.AjaxWriter;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.UrlReturnUtil;

@Controller
public class LoginController  {

	

	@Autowired
	private PassportLogin passportLogin;
	@Autowired
	private PassportTicketOpr passportTicketOpr;
	
	private ILoginService webLoginService=PassportAdaptFactory.getLoginService();
	
	//private ILoginConfigure loginConfigure = PassportAdaptFactory.getLoginConfigure();
	
	@Autowired
	private PassportCookies passportCookies;

	protected Log logger = LogFactory.getLog(this.getClass());

	protected PpLogger remoteLogger = LoginLoggerDef.login;

	/**
	 * index<br>
	 * 这个一个get,做什么用的呢?<br>
	 * 换票,主要工作
	 * 
	 * @param inv
	 * @param domain
	 * @param origURL
	 * @param p3p
	 * @param autoLogin
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 11, 2010 - 5:27:54 PM
	 */
	@RequestMapping(value = { "/Login.do", "/login.do"}, method=RequestMethod.GET)
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
			LoginStatusCode code = EventProcess.getInstance().fireBeforeLoginEvent(request, response, pForm, LoginConfigureKey.WEB_GET_LOGIN_BEFORE_LOGIN_EVENT.getKey());
			if (LoginStatusCode.LOGIN_OK != code) {
				String redirectUrl=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, code.getCode(), LoginResult.getDefualLoginResultWrapper(),LoginConfigureKey.WEB_LOGIN_FAIL_LOGIN_EVENT);
				return UrlReturnUtil.redirectUrl(response, redirectUrl);
			}
			// 用户id,判断是否来换票
			result = this.passportTicketOpr.getUserIdByPassport(
					request, response, pForm, LoginType.Web);
		} catch (Exception e) {
			// TODO: handle exception
			if(logger.isWarnEnabled()) {
				logger.warn("Before Login.do transfer method exception " + e);
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
			String redirectUrl= EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, LoginStatusCode.FAILNOPOST.getCode(), null,LoginConfigureKey.GET_WEB_LOGIN_FAIL_LOGIN_EVENT);
			return  UrlReturnUtil.redirectUrl(response,redirectUrl);
		}
		EventProcess.getInstance().fireLoginSuccessEvent(request, response, pForm, result, LoginConfigureKey.WEB_GET_LOGIN_SUCCESS_LOGIN_EVENT);
		
		String url= LoginContextUtil.getOrigUrl(pForm.getOrigURL());
		return  UrlReturnUtil.getUrlReturn(request, response, url);
	}

	/**
	 * post<br>
	 * 负责登录的方法
	 * 
	 * 目前的行为是把多个入口分布在不同的controller层中，他们的RegistrationId目前还是一致的。
	 * 但是他们在数据库中的code不一致
	 * 
	 * 以后要把所有其他的登陆入口都集中到login.do中来，
	 * 然后给不同的登陆入口分配一个RegistrationId，
	 * 再在数据库中添加新RegistrationId所对应的行为。
	 * 再把code都改成统一的名称，这样就是通过不同的RegistrationId来区分不同的行为
	 * 
	 * 现在数据库中有两套方案，一套是为了兼容目前线上登陆的入口，
	 * 可以实现不同的登陆入口都可以登陆进去，另外一套是大登陆的目标：登陆入口统一到login.do方法。
	 * 入口统一以后就可以删掉数据库中多余的配置了。
	 * 
	 * @param inv
	 * @param pForm
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 13, 2010 - 11:55:51 AM
	 */
	@RequestMapping(value = { "/login.do", "/login"}, method=RequestMethod.POST)
	public String post(@RequestParam(value = "key_id", required = false)int key_id,
	        @RequestParam(value = "domain", required = false)String domain,
            @RequestParam(value = "origURL", required = false)String origURL,
            @RequestParam(value = "errURL", required = false)String errorURL,
	        final HttpServletRequest request,
			final HttpServletResponse response,//
			final PassportForm pForm) {
	    if(key_id == 0) {
	        key_id = 1;
	    }
	    pForm.setLoginRegistrationId(key_id);
	    
	    if(origURL == null) {
            origURL = "www.renren.com/SysHome.do";
        }
        pForm.setOrigURL(origURL);
        pForm.setErrorUrl(errorURL);
        if(domain == null) {
            domain = "renren.com";
        }
        pForm.setDomain(domain);
        
		LoginStatusCode code = EventProcess.getInstance().fireBeforeLoginEvent(request, response, pForm, LoginConfigureKey.WEB_LOGIN_BEFORE_LOGIN_EVENT.getKey());
		if (LoginStatusCode.LOGIN_OK != code) {
			String url=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, code.getCode(), LoginResult.getDefualLoginResultWrapper(),LoginConfigureKey.WEB_LOGIN_FAIL_LOGIN_EVENT);
			String result=createFailJSONString(code.getCode(), code.getDescription(), 0, url, pForm.getEmail(), null);
			AjaxWriter.write(result, response);
			return null;
		}
		this.logger.debug("login posted datas are: " + pForm.toString());

		// 登录的主体
		final LoginResult result = passportLogin
				.login(pForm, true, request);
		// 登陆后
		if (result.getLoginStatusCode() == LoginStatusCode.LOGIN_OK.getCode()) {
			EventProcess.getInstance().fireLoginSuccessEvent(request, response, pForm, result, LoginConfigureKey.WEB_LOGIN_SUCCESS_LOGIN_EVENT);
			String url= LoginContextUtil.getOrigUrl( pForm.getOrigURL());
			String returnResult=createSuccessJSONString(url, result.getResultContents());
			AjaxWriter.write(returnResult, response);
			return null;
		} else {
			String url=EventProcess.getInstance().fireErrorLoginEvent(request, response, pForm, result.getLoginStatusCode(),
					result,LoginConfigureKey.WEB_LOGIN_FAIL_LOGIN_EVENT);
			String emailLoginUrl = PassportEmailUtils.emailLoginUrl(pForm.getEmail());
			emailLoginUrl = StringUtils.isEmpty(emailLoginUrl) ? "#"
					: emailLoginUrl;
			LoginResponse loginResponse=result.getLoginResponse();
			if(loginResponse==null){
				loginResponse=new LoginResponse();
			}
			String returnResult=createFailJSONString(result.getLoginStatusCode(), loginResponse.getLoginStatusdescription(), loginResponse.getCatchaCount(), url, pForm.getEmail(), emailLoginUrl);
			AjaxWriter.write(returnResult, response);
			return null;
		}
	}
	/**
	 * post<br>
	 * 负责登录的方法
	 * 
	 * @param inv
	 * @param pForm
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 13, 2010 - 11:55:51 AM
	 */
	@RequestMapping(value = { "/showCaptcha.do", "/ShowCaptcha" }, method=RequestMethod.POST)
	public String showCaptcha(final HttpServletRequest request,
			final HttpServletResponse response,//
			final PassportForm pForm) {
		int grade=webLoginService.getCaptchaGrade(pForm.getEmail(), LoginContextUtil.getClientIp(request));
		AjaxWriter.write(grade, response);
		return null;
	}

	 /**
     * 根据错误码和次数构建json字符串
     * @param code 错误代码
     * @param count 错误登录次数
     * @return 字符串
     */
    private String createFailJSONString(final int code, String description,final Integer count, final String url, final String email, final String emailLoginUrl){
        final JSONObject object=new JSONObject();
        object.put("catchaCount", count);
        object.put("code", Boolean.FALSE);
        object.put("homeUrl", url);
		object.put("failDescription", description);
		object.put("failCode",code);
		object.put("email", email);
		object.put("emailLoginUrl", emailLoginUrl);
        return object.toString();
    }
    /**
     * 根据错误码和次数构建json字符串
     * @param code 错误代码
     * @param count 错误登录次数
     * @return 字符串
     */
    private String createSuccessJSONString(final String url, final Map<String, Object> resultContent){
        final JSONObject object=new JSONObject();
        object.put(LoginResultCode.CODE, Boolean.TRUE);
        object.put(LoginResultCode.HOME_URL, url);
        Iterator<Entry<String, Object>> it = resultContent.entrySet().iterator();
        while(it.hasNext()){
        	Entry<String, Object> entry=it.next();
        	String key=entry.getKey();
        	Object value=entry.getValue();
        	object.put(key, value);
        }
        return object.toString();
    }
    
   
}
