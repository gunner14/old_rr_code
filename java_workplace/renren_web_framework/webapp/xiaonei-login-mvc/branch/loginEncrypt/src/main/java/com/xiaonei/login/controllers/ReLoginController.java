package com.xiaonei.login.controllers;

import java.net.URL;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.xce.passport.PassportAdapterFactory;

import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 用于实现对passport域的p票清理的工作
 * 
 * @author blackwing
 * 
 */
@Path(value = { "Relogin.do", "relogin.do" })
@IgnorePassportValidation
public class ReLoginController {

    @Post
    @Get
    public String index(Invocation inv, // NL
            @Param("domain") @DefValue("renren.com") String domain, //
            @Param(value = "origURL") String origUrl //
    ) {
        String passport = CookieManager.getInstance().getCookie(inv.getRequest(),
                LoginCookieDef.COOKIE_PASSPORT);
        if (passport != null) {
            CookieManager.getInstance().clearCookie(inv.getResponse(),
                    LoginCookieDef.COOKIE_PASSPORT, 0, "/", Globals.PASSPORT_DOMAIN);
            try {
				PassportAdapterFactory.getWebLogin().destroyByTicket(passport);
            } catch (Ice.TimeoutException e) {
            	try {
					PassportAdapterFactory.getWebLogin().destroyByTicket(passport);
				} catch (Exception e1) {
					e.printStackTrace();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
        }
        if (domain != null && domain.trim().length() > 0) {
            if (!"renren.com".equals(domain)&&!"kaixin.com".equals(domain)) {
                domain = OpiConstants.getCurrentDomain();;
            }
            inv.getRequest().setAttribute("domain", domain);
        } else {
            domain = "renren.com";
        }
        if (origUrl != null && origUrl.trim().length() > 0) {
        	//避免跳到站外
      		try {
      			URL callBackUrl = new URL(origUrl);
      			if(callBackUrl.getHost().indexOf(OpiConstants.getCurrentDomain()) == -1){
      				if(! callBackUrl.getHost().contains("56.com")){
      					origUrl = OpiConstants.urlMain.toString();
      				}
      			}	
      		} catch (Exception e) {
      			
      		}
        	
            return "r:" + origUrl;
        }
        return "r:" + "http://www." + domain + "/SysHome.do";
    }
}
