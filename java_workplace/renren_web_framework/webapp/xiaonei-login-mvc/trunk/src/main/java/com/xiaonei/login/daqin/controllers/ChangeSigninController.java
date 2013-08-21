package com.xiaonei.login.daqin.controllers;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.controllers.base.RemoteAddrFetherImpl;
import com.xiaonei.login.daqin.services.DaqinService;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.logic.ticket.PassportTicketOpr;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.Login.Type;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Path("c-signin")
@DontPutOrigURL
@IgnoreUserStatusImportedValidation
@IgnorePassportValidation
public class ChangeSigninController {
	 public static void main(final String[] args) {
        final List<Account> accounts = PassportAdapterFactory.getWebLogin().get(
                "cn.wang.tai@gmail.com", Type.Account);
        System.out.println(accounts);
    }
    protected PpLogger remoteLogger = LoginLoggerDef.login;
    final protected RemoteAddrFetherImpl addrFetherImpl = new RemoteAddrFetherImpl();
    @Autowired
    private DaqinService daqinService;

    @Autowired
    private PassportTicketOpr passportTicketOprImpl;

    @Get
    public String get(final Invocation inv, @HtmlPure @Param("d") final String domain,
            @HtmlPure @Param("u") final String url) {
        if (!this.checkDomain(domain)) {
            return "@fail";
        }
        final String callbackUrl = this.daqinService.getCallbackUrl(domain);
        if (StringUtils.isEmpty(callbackUrl)) {
            return "@fail";
        }
        final String ip = this.addrFetherImpl.getRemoteAddr(inv.getRequest());
        final LoginSourceType loginSourceType = this.getLoginSourceType(domain);
        final String referer = inv.getRequest().getHeader("Referer");
        // 用户id,判断是否来换票
        final int hostid = this.passportTicketOprImpl.getUserIdByPassport(inv.getRequest(),
                inv.getResponse());
        if (hostid > 0) {
            final UserPassport passport = SnsAdapterFactory.getUserPassportAdapter()
                    .getUserPassport(hostid);
            final Account account = new Account(passport.getId(), 0, passport.getAccount(),
                    passport.getDomain(), passport.getStatus(), "");
            final List<Account> accounts = Arrays.asList(account);
            if (accounts == null) {
                return "r:" + callbackUrl + "?p=fail";
            }
           
            this.remoteLogger.info(LoginLoggerDef.loginLogPentterNew, "autoLoginOk",
            		account != null ? account.getId() : 0, referer, account != null ?account.getAccount():"", ip, loginSourceType.getValue());
            final String json = this.daqinService.getReturnJson(domain, accounts, url);
            return "r:" + callbackUrl + "?p=" + json;
        } else {
            final List<Account> accounts = Collections.emptyList();
            final String json = this.daqinService.getReturnJson(domain, accounts, url);
            return "r:" + callbackUrl + "?p=" + json;
        }
    }
    private boolean checkDomain(final String domain) {
        return !StringUtils.isEmpty(domain);
    }
    private LoginSourceType getLoginSourceType(final String domain) {
        if (StringUtils.isEmpty(domain)) {
            return LoginSourceType.DaQin;
        }
        if ("fengche.com".equals(domain)) {
            return LoginSourceType.WEBLOGIN_FENGCHE;
        } else if ("56.com".equals(domain)) {
            return LoginSourceType.WEBLOGIN_56;
        } else {
            return LoginSourceType.DaQin;
        }
    }
}
