package com.xiaonei.passport.web.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;

import com.renren.passport.service.ILoginService;
import com.xiaonei.aegis.topusing.AegisLoginConstants;
import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLogger;
import com.xiaonei.passport.model.Account;
import com.xiaonei.passport.passport.IPassportService;
import com.xiaonei.passport.passport.PassportServiceFactory;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.passport.ticket.TicketServiceFactory;
import com.xiaonei.passport.web.adapt.PassportAdaptFactory;
import com.xiaonei.passport.web.constants.LoginCookieDef;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;
import com.xiaonei.xce.ticket.TicketInfo;

@Path("c-signin")
@DontPutOrigURL
@IgnoreUserStatusImportedValidation
@IgnorePassportValidation
public class ChangeSigninController {
	
	protected PpLogger remoteLogger = LoginLoggerDef.login;
	private ILoginService loginService=PassportAdaptFactory.getLoginService();
	private ITicketService ticketService = TicketServiceFactory.getInstance();
	private IPassportService passportService = PassportServiceFactory.getInstance();
	
	@Get
    public String get(final Invocation inv, @HtmlPure @Param("d") final String domain,
            @HtmlPure @Param("u") final String url) {
		String callbackUrl = getCallbackUrl(domain);
		if(StringUtils.isEmpty(domain) || StringUtils.isEmpty(callbackUrl)){
			return "@fail";
		}
		final String ip = LoginContextUtil.getClientIp(inv.getRequest());
		final int loginSourceType = this.getLoginSourceType(domain);
		final String referer = inv.getRequest().getHeader("Referer");
		// 用户id,判断是否来换票
		String npTicket = CookieManager.getInstance().getCookie(inv.getRequest(), LoginCookieDef.PASSWORD);
		TicketInfo info = ticketService.getTicketInfo(npTicket, TicketType.PASSPORT);
		if (info == null || info.get_id() <= 0) {
			return getReturnJson(domain, null, url);
			/*return LogicBeanProcessor.processLogicAfterError(LoginStatusCode.FAILNOPOST,
					loginParameter, LoginConfigureKey.TRANSFER_LOGIC_FAIL_LOGIN.getKey());*/
		}
		
		Account account = passportService.getAccount(info.get_id());
		if (account == null) {
            return "r:" + callbackUrl + "?p=fail";
        }
		
		this.remoteLogger.info("status:%s;account:%s;desc:%s;email:%s;ip:%s;type:%s", "autoLoginOk",
        		account != null ? account.getId() : 0, referer, account != null ?account.getAccount():"", ip, loginSourceType);
		
		final String json = getReturnJson(domain, account, url);
		return "r:" + callbackUrl + "?p=" + json;
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
	
    private String getReturnJson(final String domain, Account account, final String url) {
        final int time = (int) (System.currentTimeMillis() / 1000);
        final StringBuffer str = new StringBuffer();
        str.append(time);
        str.append("-");
        if(account != null){
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
        
        str.append("-");
        str.append(this.getSigninKey(domain));
        final String hashcode = Encoder.encode(str.toString());
        final StringBuffer psb = new StringBuffer();
        psb.append(time).append(hashcode);
        
        psb.append(",");
        if(account != null){
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
}
