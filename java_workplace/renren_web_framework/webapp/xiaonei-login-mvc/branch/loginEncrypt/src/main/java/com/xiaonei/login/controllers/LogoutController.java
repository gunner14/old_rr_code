package com.xiaonei.login.controllers;

import java.net.URL;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLoggerArgTask;
import com.xiaonei.login.logic.ticket.PassportCookiesImpl;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;

@Path(value = { "logout.do", "Logout.do" })
@IgnorePassportValidation
public class LogoutController {

    @Autowired
    private PassportCookiesImpl passportCookiesImpl;

    @Post
    public String index(final Invocation inv) {
        return "";
    }

    @Get
    public String index(final Invocation inv,
            @Param(value = "get_check") @DefValue("") final String getCheck,
            @Param(value = "origURL") @DefValue("") String origURL) {
        final StatUtil su = new StatUtil();
        String checkRt;
        if ((checkRt = this.checkCsrfRt(inv, getCheck)) != null) {
            String utf8url;
            try {
                utf8url = URLEncoder.encode(origURL, "UTF-8");
            } catch (final Exception e) {
                utf8url = origURL;
            }
            return StringUtils.isNotEmpty(origURL) ? checkRt + "&origURL=" + utf8url : checkRt;
        }
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
        
        LoginLoggerDef.logout.trace("origURL:" + origURL);

        this.clearAutoLoginCookie(inv.getResponse());
        LoginLoggerDef.logout.trace("clearAutoLoginCookie");
        this.clearHostId(inv);
        LoginLoggerDef.logout.trace("clearHostId");
        su.stepTimeCost("1:LogoutAction:CleanTicket");

        su.getWholeTime("LogoutSuccFinish");

        return "r:" + OpiConstants.urlPassport + "/relogin.do" // NL
        + "?domain=" + OpiConstants.getCurrentDomain() // NL
        + (origURL != null ? "&origURL=" + origURL : "");
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
    private String checkCsrfRt(final Invocation inv, final String getCheck) {
        final String ticket = CookieManager.getInstance().getCookie(inv.getRequest(),
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
                    LoginLoggerDef.logout.info(LoginLoggerDef.loginLogPentter, "Logout Succ!!!",
                            this.getHostId(inv), "");
                    return null;
                } else {
                    LoginLoggerDef.logout.warn(LoginLoggerDef.loginLogPentter, "Logout CSRF Error",
                            this.getHostId(inv), inv.getRequest().getHeader("Referer"));
                    return "r:" + OpiConstants.urlLogin + "/logout.do?get_check="
                    + ticket.hashCode();
                }

            }
        } else {
            LoginLoggerDef.logout.warn(LoginLoggerDef.loginLogPentter, "Logout Url Error",
                    this.getHostId(inv), inv.getRequest().getHeader("Referer"));
            return "r:" + OpiConstants.urlLogin + "/logout.do?get_check=" + ticket.hashCode();
        }
        return null;
    }

    private void clearAutoLoginCookie(final HttpServletResponse response) {
        this.passportCookiesImpl.clearAutoLoginCookie(response);
    }

    private void clearHostId(final Invocation inv) {
        try {
            PassportManager.getInstance().clearHostId(inv.getRequest(), inv.getResponse());
        } catch (final Ice.ConnectTimeoutException e) {
            LoginLoggerDef.logout.error("clear host timeout", e);
        } catch (final Ice.ConnectionTimeoutException e) {
            LoginLoggerDef.logout.error("clear host timeout", e);
        } catch (final Ice.TimeoutException e) {
            LoginLoggerDef.logout.error("clear host timeout", e);
        } catch (final Exception e) {
            LoginLoggerDef.logout.error("clear host err", e);
            e.printStackTrace();
        }
    }

    /**
     * getHostId<br>
     * 
     * @param inv
     * @return
     * 
     * @author tai.wang@opi-corp.com Jan 28, 2010 - 11:37:34 AM
     */
    private PpLoggerArgTask getHostId(final Invocation inv) {
        return new PpLoggerArgTask() {

            @Override
            public Object work() {
                return PassportManager.getInstance().getHostIdFromCookie(inv.getRequest(),
                        inv.getResponse());
            }
        };
    }

}
