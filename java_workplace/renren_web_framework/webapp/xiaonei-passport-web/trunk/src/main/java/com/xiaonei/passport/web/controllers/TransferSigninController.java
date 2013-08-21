package com.xiaonei.passport.web.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.aegis.topusing.AegisLoginConstants;
import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.model.Account;
import com.xiaonei.passport.passport.IPassportService;
import com.xiaonei.passport.passport.PassportServiceFactory;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.passport.PassportLogin;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;

@Path("{t-signin, t-signin2}")
@DontPutOrigURL
@IgnoreUserStatusImportedValidation
@IgnorePassportValidation
@AsSuperController
public class TransferSigninController {
	
	private static final String LOGFORMAT = "status:%s;account:%s;desc:%s;email:%s;ip:%s;type:%s";
	protected PpLogger remoteLogger = LoginLoggerDef.login;
	
	@Autowired
	private PassportLogin passportLogin;
	
	private IPassportService passportService = PassportServiceFactory.getInstance();
	
	@Post
    public String post(final Invocation inv, @HtmlPure @Param("e") final String email,
            @Param("p") final String password, @HtmlPure @Param("d") final String domain,
            @HtmlPure @Param("u") final String url) {
		HttpServletRequest request = inv.getRequest();
		final String ip = LoginContextUtil.getClientIp(request);
		request.setAttribute("encodePassword", password);
		final String referer = request.getHeader("Referer");
		final int sourceType = getLoginSourceType(domain);
		if(StringUtils.isEmpty(email) || StringUtils.isEmpty(password)){
			this.remoteLogger.info(String.format(LOGFORMAT, "您的用户名和密码不匹配", email, referer, email, ip, sourceType));
            return "@fail";
		}
		if(StringUtils.isEmpty(domain)){
			this.remoteLogger.info(String.format(LOGFORMAT, "您输入的参数不正确", email, referer, email, ip, sourceType));
            return "@fail";
		}
		//TODO why remove ? why t?  (a special requirement...at all, to keep a clean env)
		//only remove t  ticket
        CookieManager.getInstance().clearCookie(inv.getResponse(), "t", 0, "renren.com");
        final String callbackUrl = getCallbackUrl(domain);
        if (StringUtils.isEmpty(callbackUrl)) {
            this.remoteLogger.info(String.format(LOGFORMAT, "您输入的参数不正确", email, referer, email, ip, sourceType));
            return "@fail";
        }
        
        final String md5Password = Encoder.encode(password);
        final LoginResult result = passportLogin.login(email, md5Password, false, null, request.getParameter("icode"), 
        		request.getParameter("captcha_type"), null, domain, 0, 1, true, request);
        if(result.getLoginStatusCode() == LoginStatusCode.LOGIN_OK.getCode()){
        	
        	//在上线期间可能会有转换错误，这里兼容一下
        	Account mainAccount = null;
        	Object whichAccount = request.getAttribute("account");
        	if(whichAccount instanceof com.xiaonei.xce.passport.Account){
        		com.xiaonei.xce.passport.Account temp = (com.xiaonei.xce.passport.Account)whichAccount;
        		mainAccount = new Account(temp.getId(), temp.getAccount(), temp.getDomain(), temp.getStatus(), temp.getId(), 0);
        	} else {
        		try {
        			mainAccount = (Account) request.getAttribute("account");        							
				} catch (Exception e) {
					
				}
        	}
        	
            this.remoteLogger.info(LOGFORMAT, "loginOk",
                    mainAccount != null ? mainAccount.getId() : 0, referer, email, ip, sourceType);
            
            List<Account> returnAccount = new ArrayList<Account>();
            if(mainAccount != null){
	            returnAccount.add(mainAccount);
	            if("56.com".equals(domain) && !"56.com".equals(mainAccount.getDomain())){
	            	List<Account> accounts = passportService.getMatchedAccount(mainAccount.getAccount(), md5Password);
	            	for(Account item : accounts){
	            		if(item.getId() == mainAccount.getId()){
	            			continue;
	            		}
	            		if("56.com".equals(item.getDomain())){
	            			returnAccount.add(item);
	            			break;
	            		}
	            	}
	            }
            }
            
            final String json = getReturnJson(domain, returnAccount, url);
            return "r:" + callbackUrl + "?p=" + json;
        } else {
        	this.remoteLogger.info(String.format(LOGFORMAT, result.getResultContents(),
                    email, referer, email, ip, sourceType));
        	final String json = getReturnJson(domain, null, url);
        	return "r:" + callbackUrl + "?p=" + json+"&code="+result.getLoginStatusCode();
        }
        
	}
	
	protected LoginResult md5Login(String email, String md5Password, HttpServletRequest request, String domain){
		return passportLogin.login(email, md5Password, false, null, request.getParameter("icode"), 
        		request.getParameter("captcha_type"), null, domain, 0, 1, true, request);
	}
	
	private String getReturnJson(final String domain, List<Account> accounts, final String url) {
        final int time = (int) (System.currentTimeMillis() / 1000);
        final StringBuffer str = new StringBuffer();
        str.append(time);
        str.append("-");
        if(accounts != null){
        	for(Account account : accounts){
	        	str.append(account.getId());
	            str.append("-");
	            str.append(account.getDomain());
	            str.append("-");
	            str.append(account.getStatus());
	            if ("56.com".equals(account.getDomain())) {
	                str.append("-");
	                str.append(account.getAccount());
	            }
        	}
        }
        
        str.append("-");
        str.append(this.getSigninKey(domain));
        final String hashcode = Encoder.encode(str.toString());
        final StringBuffer psb = new StringBuffer();
        psb.append(time).append(hashcode);
        
        psb.append(",");
        if(accounts != null){
        	for(Account account : accounts){
	        	psb.append(account.getId());
	            psb.append("-");
	            psb.append(account.getDomain());
	            psb.append("-");
	            psb.append(account.getStatus());
	            if ("56.com".equals(account.getDomain())) {
	                psb.append("-");
	                psb.append(account.getAccount());
	            }
        	}
        }
        
        final JSONObject jo = new JSONObject();
        jo.put("p", psb.toString());
        jo.put("u", url);
        try {
            return URLEncoder.encode(jo.toString(), "UTF-8");
        } catch (final UnsupportedEncodingException e) {
            e.printStackTrace();
            return jo.toString();
        }
    }
	
	private String getSigninKey(final String domain) {
	    if ("nuomi.com".equals(domain)) {
	        return "305880804";
	    } else if ("fengche.com".equals(domain)) {
	        return "QingTing";
	    } else if ("56.com".equals(domain)) {
	        return "5556666";
	    }
	    return "@";
	}
	
	private int getLoginSourceType(final String domain) {
        if (StringUtils.isEmpty(domain)) {
            return AegisLoginConstants.LoginType.WEBLOGIN_DaQin.ordinal();
        }
        if ("fengche.com".equals(domain)) {
            return AegisLoginConstants.LoginType.WEBLOGIN_FENGCHE.ordinal();
        } else if ("56.com".equals(domain)) {
            return AegisLoginConstants.LoginType.WEBLOGIN_56.ordinal();
        } else {
            return AegisLoginConstants.LoginType.WEBLOGIN_DaQin.ordinal();
        }
    }
	
    private String getCallbackUrl(final String domain) {
        if ("nuomi.com".equals(domain)) {
            return "http://www.nuomi.com/renren/callback";
        } else if ("fengche.com".equals(domain)) {
            return "http://www.fengche.com/renren/callback";
        } else if ("56.com".equals(domain)) {
            return "http://user.56.com/login/callback";
        }
        return null;
    }
}
