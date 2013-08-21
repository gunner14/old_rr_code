package com.xiaonei.login.http;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.antispam.AntiSpamUtils;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.xce.passport.PassportAdapterFactory;

public class ReLoginAction extends Action {

    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws Exception {
        StatUtil su = new StatUtil();
        String passport = CookieManager.getInstance().getCookie(request,
                LoginAction.COOKIE_PASSPORT);
        String domain = request.getParameter("domain");
        String origURL = request.getParameter("origURL");
        if (passport != null) {
            CookieManager.getInstance().clearCookie(response, LoginAction.COOKIE_PASSPORT, 0, "/",
                    Globals.PASSPORT_DOMAIN);
            PassportAdapterFactory.getWebLogin().destroyByTicket(passport);
            su.stepTimeCost("3:ReLoginAction:destroyTicket");
        }
        if (domain != null && domain.trim().length() > 0) {
            if (!AntiSpamUtils.getInstance().isSafeUrl(domain)) {
                domain = OpiConstants.getCurrentDomain();;
            }
            request.setAttribute("domain", domain);
        } else {
            domain = "renren.com";
        }
        su.getWholeTime("ReLoginSuccFinish");
        if (origURL != null && origURL.trim().length() > 0) {
            return new ActionForward(origURL, true);
        }
        return new ActionForward("http://www." + domain + "/SysHome.do", true);
    }
}
