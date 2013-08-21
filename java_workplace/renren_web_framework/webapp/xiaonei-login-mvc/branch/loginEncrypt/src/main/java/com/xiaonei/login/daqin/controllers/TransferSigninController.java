package com.xiaonei.login.daqin.controllers;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.antispam.validator.HtmlPure;
import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.controllers.base.RemoteAddrFetherImpl;
import com.xiaonei.login.daqin.services.DaqinService;
import com.xiaonei.login.logic.PassportLoginImpl;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChainsImpl;
import com.xiaonei.login.logic.afterlogin_chains.node.AccountChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.CheckUserStatusResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SafeCheckResultChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.SaveTicketChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.TaskAfterLoginChainNode;
import com.xiaonei.login.logic.afterlogin_chains.node.WeakPasswordChainNode;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Path("t-signin")
@DontPutOrigURL
@IgnoreUserStatusImportedValidation
@IgnorePassportValidation
@AsSuperController
public class TransferSigninController implements InitializingBean {

    final protected RemoteAddrFetherImpl addrFetherImpl = new RemoteAddrFetherImpl();

    @Autowired
    private AccountChainNode accountChainNode;

    private final LoginResultChains chains = new LoginResultChainsImpl();

    @Autowired
    private DaqinService daqinService;

    @Autowired
    private PassportLoginImpl passportLoginImpl;

    @Autowired
    private SafeCheckResultChainNode safeCheckResultChainNode;

    @Autowired
    private SaveTicketChainNode saveTicketChainNode;

    @Autowired
    private CheckUserStatusResultChainNode checkUserStatusResultChainNode;
    @Autowired
    private TaskAfterLoginChainNode taskAfterLoginChainNode;

    @Autowired
    private WeakPasswordChainNode weakPasswordChainNode;

    protected PpLogger remoteLogger = LoginLoggerDef.login;

    @Override
    public final void afterPropertiesSet() throws Exception {
        if (this.chains.isEmpty()) {
            this.chains.setNode(this.safeCheckResultChainNode);
            this.chains.setNode(this.accountChainNode);
            this.chains.setNode(this.checkUserStatusResultChainNode);
            this.chains.setNode(this.taskAfterLoginChainNode);
            this.chains.setNode(this.weakPasswordChainNode);
            this.chains.setNode(this.saveTicketChainNode);
        }
    }

    @Post
    public String post(final Invocation inv, @HtmlPure @Param("e") final String email,
            @Param("p") final String password, @HtmlPure @Param("d") final String domain,
            @HtmlPure @Param("u") final String url) {
        final String ip = this.addrFetherImpl.getRemoteAddr(inv.getRequest());
        inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD, password);
        final String referer = inv.getRequest().getHeader("Referer");
        final LoginSourceType loginSourceType = this.getLoginSourceType(domain);
        inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE, loginSourceType);
        final int sourceType = loginSourceType.getValue();
        if (!this.checkEmailAndPwd(email, password)) {
            this.remoteLogger.info(String.format(LoginLoggerDef.loginLogPentterNew,
                    LoginStatusCode.FAILEMPTYACOUNT.name(), email, referer, email, ip, sourceType));
            return "@fail";
        }
        if (!this.checkDomain(domain)) {
            this.remoteLogger.info(String.format(LoginLoggerDef.loginLogPentterNew,
                    LoginStatusCode.FAILINPUT.name(), email, referer, email, ip, sourceType));
            return "@fail";
        }
        //only remove t  ticket
        CookieManager.getInstance().clearCookie(inv.getResponse(), LoginCookieDef.loginKey_ticket,
                0, Globals.PASSPORT_DOMAIN);
        final String callbackUrl = this.daqinService.getCallbackUrl(domain);
        if (StringUtils.isEmpty(callbackUrl)) {
            this.remoteLogger.info(String.format(LoginLoggerDef.loginLogPentterNew,
                    LoginStatusCode.FAILINPUT.name(), email, referer, email, ip, sourceType));
            return "@fail";
        }
        final String md5Password = Encoder.encode(password);
        final LoginStatusCode code = this.md5login(inv, email, md5Password, domain);
        if (code == LoginStatusCode.LOGIN_OK) {
            final Account mainAccount = (Account) inv.getRequest().getAttribute(
                    LoginParamDef.ATTRIBUTE_ACCOUNT);
            this.remoteLogger.info(LoginLoggerDef.loginLogPentterNew, "loginOk",
                    mainAccount != null ? mainAccount.getId() : 0, referer, email, ip, sourceType);
            final List<Account> accounts = this.getAccounts(inv, domain);
            if (null == accounts) {
                return "r:" + callbackUrl + "?e=" + email;
            }
            final String json = this.daqinService.getReturnJson(domain, accounts, url);
            return "r:" + callbackUrl + "?p=" + json;
        } else {
            this.remoteLogger.info(String.format(LoginLoggerDef.loginLogPentterNew, code.name(),
                    email, referer, email, ip, sourceType));
            final List<Account> accounts = Collections.emptyList();
            final String json = this.daqinService.getReturnJson(domain, accounts, url);
            return "r:" + callbackUrl + "?p=" + json+"&code="+code.getCode();
        }
    }

    private boolean checkDomain(final String domain) {
        return !StringUtils.isEmpty(domain);
    }

    private boolean checkEmailAndPwd(final String email, final String md5Password) {
        return !StringUtils.isEmpty(email) && !StringUtils.isEmpty(md5Password);
    }

    /**
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Dec 4, 2011 12:01:46 PM
     * 
     * @param inv
     * @param domain //不同网站的给你路处理逻辑不一样。
     * @return 如果其它帐号中有56.com就返回多个，否则只返回一个。
     */
    private List<Account> getAccounts(final Invocation inv, final String domain) {
        final List<Account> as = new ArrayList<Account>();
        final Account mainAccount = (Account) inv.getRequest().getAttribute(
                LoginParamDef.ATTRIBUTE_ACCOUNT);
        as.add(mainAccount);//加上默认帐号
        UserPassport up2 = null;
        if ("56.com".equals(domain)) { //56.com逻辑特殊处理
            if ("56.com".equals(mainAccount.getDomain())) { // 如果取出来的就是56.com帐号，直接返回
                return as;
            }
            final List<Account> allAccounts = PassportAdapterFactory.getWebLogin().getAccount(
                    mainAccount.getAccount());
            if ((allAccounts == null) || (allAccounts.size() <= 1)) { // 出错，直接返回
                return as;
            }
            for (final Account account : allAccounts) {
                if (account.getId() == mainAccount.getId()) { // 不重复加上帐号
                    continue;
                }
                if ("56.com".equals(account.getDomain())) { // 如果其它帐号是56.com就加上一个。
                    final UserPassport up1 = SnsAdapterFactory.getUserPassportAdapter()
                            .getUserPassport(account.getId());
                    if (up2 == null) {
                        up2 = SnsAdapterFactory.getUserPassportAdapter().getUserPassport(
                                mainAccount.getId());
                    }
                    if (up2.getPasswordMd5().equals(up1.getPasswordMd5())) {
                        as.add(account);
                    }
                    continue;
                }
            }
        }
        return as;
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

    protected LoginStatusCode md5login(final Invocation inv, final String email,
            final String md5Password, final String domain) {
        inv.getRequest().setAttribute(LoginParamDef.PARAM_ENPWD, Boolean.TRUE);
        return this.passportLoginImpl.login(inv.getRequest(), inv.getResponse(), false, email,
                md5Password, domain, this.chains);
    }
}
