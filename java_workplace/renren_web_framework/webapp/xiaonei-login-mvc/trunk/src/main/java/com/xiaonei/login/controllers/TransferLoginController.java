package com.xiaonei.login.controllers;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;

import javax.servlet.http.HttpServletRequest;

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
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.logic.DomainOprImpl;
import com.xiaonei.login.logic.ticket.PassportTicketOprImpl;
import com.xiaonei.login.task.RunnableTaskExecutor;
import com.xiaonei.login.task.task.LastIpTask;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Path("transfer.do")
@IgnorePassportValidation
public class TransferLoginController {

    private static class ValidResult {

        public boolean isValid = false;

        public String result;

        public String ticket;

        public int userId;
    }

    @Autowired
    private PassportTicketOprImpl passportTicketOprImpl;

    @Autowired
    private DomainOprImpl domainOprImpl;

    @Post
    @Get
    public String index(final Invocation inv, //
            @Param("domain") @DefValue("renren.com") String domain, //
            @Param("transfer") final String transfer,//
            @Param("origURL") String origURL,//
            @Param("p3p") final String p3p,// 
            @Param("autoLogin") @DefValue("false") final boolean autoLogin,//
            @Param("id") int userId,//
            @Param("time") final long time,//
            @Param("m") final String md5//
    ) {

        final ValidResult rt = this
                .validate(origURL, transfer, userId, md5, time, inv.getRequest());
        if (!rt.isValid) {
            return rt.result;
        }
        userId = rt.userId;
        try {
            domain = this.domainOprImpl.getDomainParam(domain, inv.getRequest());
            final StatUtil su = new StatUtil();
            final User user = WUserAdapter.getInstance().get(userId);
            su.stepTimeCost("TransferAction:getUser");

            final Account account = PassportAdapterFactory.getWebLogin().login(
                    com.xiaonei.login.http.AbstractStructAction.defaultLogin, user.getAccount(),
                    user.getPassword_md5(), domain, -1, true);
            su.stepTimeCost("TransferAction:createPassport");

            this.passportTicketOprImpl.savePassport2Cookie(userId, inv.getRequest(), inv
                    .getResponse(), account.getTicket(), false);
            final String ticket = this.createTicket(rt.ticket, userId, inv.getRequest());

            inv.getRequest().setAttribute(LoginParamDef.PARAM_DOMAIN, domain);
            inv.getRequest().setAttribute(LoginParamDef.PARAM_ORIGURL, origURL);
            CookieManager.getInstance().saveCookie(inv.getResponse(), LoginCookieDef.TRANSFER,
                    transfer);

            RunnableTaskExecutor.getInstance().runTaskForFM(
                    new LastIpTask(userId, BizFilterUtil.getClientIP(inv.getRequest()), 4));

            return "r:"
                    + this.passportTicketOprImpl.forwardSetTicket(inv.getRequest(), ticket, domain,
                            origURL, p3p, autoLogin, true);
        } catch (final Exception e) {
            e.printStackTrace();
            if (origURL.indexOf('?') == -1) {
                origURL += "?errCode=" + "0";
            } else {
                origURL += "&errCode=" + "0";
            }
        }
        return "r:" + origURL;
    }

    private String createTicket(final String ticket, final int userId,
            final HttpServletRequest request) {
        if (StringUtils.isNotEmpty(ticket)) {
            return ticket;
        }
        return PassportManager.getInstance().createTicket(userId, request);
    }

    private ValidResult validate(final String origURL, final String transfer, final int userId,
            final String md5, final long time, final HttpServletRequest request) {
        final ValidResult rt = new ValidResult();
        rt.userId = userId;
        if (origURL == null || origURL.trim().length() == 0) {
            return rt;
        }
        if (StringUtils.isNotEmpty(transfer)) {
            return this.validateTicket(origURL, transfer, rt);
        } else if (userId != 0 || time != 0l || StringUtils.isNotEmpty(md5)) {
            return this.validateEncodeString(origURL, userId, md5, time, request, rt);
        } else {
            return this.validateFailed(origURL, rt);
        }

    }

    private ValidResult validateEncodeString(final String origURL, final int userId,
            final String md5, final long time, final HttpServletRequest request,
            final ValidResult rt) {
        final long timeDiff = (System.currentTimeMillis() / 1000 - time) / 60;
        if (timeDiff > 5) {
            return this.validateFailed(origURL, rt);
        }
        final String ticket = this.createTicket(null, userId, request);
        if (StringUtils.isEmpty(ticket)) {
            rt.ticket = ticket;
            return this.validateFailed(origURL, rt);
        }
        if (md5.equals(Encoder.encode(ticket + time).substring(0, 16))) {
            rt.isValid = true;
            rt.userId = userId;
            return rt;
        } else {
            return this.validateFailed(origURL, rt);
        }
    }

    private ValidResult validateFailed(String origURL, final ValidResult rt) {
        rt.isValid = false;
        if (null == origURL) {
            origURL = OpiConstants.urlHome.toString();
        }
        try {
            origURL = URLDecoder.decode(origURL, "UTF-8");
        } catch (final UnsupportedEncodingException e) {
            e.printStackTrace();
            try {
                new URL(origURL);
            } catch (final MalformedURLException e1) {
                e1.printStackTrace();
                origURL = OpiConstants.urlHome.toString();
            }
        }
        rt.result = "r:" + origURL;
        return rt;
    }

    private ValidResult validateTicket(final String origURL, final String transfer,
            final ValidResult rt) {
        final int userid = PassportAdapterFactory.getTransfer().verifyTicket(transfer);
        if (userid <= 0) {
            return this.validateFailed(origURL, rt);
        } else {
            rt.isValid = true;
            rt.userId = userid;
            return rt;
        }
    }
}
