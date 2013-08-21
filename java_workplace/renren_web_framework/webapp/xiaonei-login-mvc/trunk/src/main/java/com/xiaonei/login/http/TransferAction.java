package com.xiaonei.login.http;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;

public class TransferAction extends Action {

    @Override
    public ActionForward execute(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) throws Exception {
        String domain = request.getParameter("domain");
        final String origUrl = request.getParameter("origURL");
        final String transfer = request.getParameter("transfer");

        if (origUrl != null && origUrl.trim().length() > 0) {
            if (transfer != null && transfer.trim().length() > 0) {
                try {
                    final int userid = PassportAdapterFactory.getTransfer().verifyTicket(transfer);
                    if (userid > 0) {
                        try {
                            if (domain == null) {
                                domain = origUrl.contains("kaixin.com") ? "kaixin.com"
                                        : "renren.com";
                            }
                            final User user = WUserAdapter.getInstance().get(userid);

                            final Account account = PassportAdapterFactory.getWebLogin().login(
                                    com.xiaonei.login.http.AbstractStructAction.defaultLogin,
                                    user.getAccount(), user.getPassword_md5(), domain, -1, true);

                            LoginLogicFactory.getPassportTicketOpr().savePassport2Cookie(userid,
                                    request, response, account.getTicket(), false);
                            final String ticket = PassportManager.getInstance().createTicket(
                                    userid, request);
                            request.setAttribute(LoginAction.PARAM_DOMAIN, domain);
                            request.setAttribute(LoginAction.PARAM_ORIGURL, origUrl);
                            CookieManager.getInstance().saveCookie(response, "transfer", transfer);

                            return new ActionForward(LoginLogicFactory.getPassportTicketOpr()
                                    .forwardSetTicket(
                                            request,
                                            ticket,
                                            domain,
                                            request.getParameter(LoginParamDef.PARAM_ORIGURL),
                                            request.getParameter("p3p"),
                                            Boolean.valueOf(request
                                                    .getParameter(LoginParamDef.PARAM_AUTOLOGIN)),
                                            true), true);
                        } catch (final Exception e) {
                            e.printStackTrace();
                        }
                    }
                } catch (final Exception e) {
                    e.printStackTrace();
                }
                return new ActionForward(origUrl, true);
            }
        }
        return null;
    }
}
