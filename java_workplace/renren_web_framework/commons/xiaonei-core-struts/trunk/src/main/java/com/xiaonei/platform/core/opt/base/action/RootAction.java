package com.xiaonei.platform.core.opt.base.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.filter.SafeCenterFilter;
import com.xiaonei.platform.core.opt.base.chain.impl.handler.SafeCenterHandler;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.xce.log.TimeCost;

/**
 * 校内action 框架的根其他的action 直接或间接继承 RootAction 该action 完成用户票验证 以及用户
 * 
 * @author yunlong.bai@opi-corp.com
 * 
 */
public abstract class RootAction extends Action {

    private static final Log RootLogger = LogFactory.getLog("core");

    public abstract ActionForward executeR(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws Exception;

//    /**
//     * 获取上次请求的referer
//     * 
//     * @param request
//     * @return
//     */
//    protected String getCookieBeforeReferer(HttpServletRequest request) {
//        return CookieManager.getInstance().getCookie(request, OpiConstants.BEFORE_REFERER);
//    }
//
//    protected String getCookieReferer(HttpServletRequest request) {
//        String referer = CookieManager.getInstance().getCookie(request, OpiConstants.REFERER);
//        if (referer == null) {
//            referer = request.getHeader("Referer");
//        }
//        return referer;
//    }

    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) {

        TimeCost timeLog = TimeCost.begin(RootLogger, "RootAction");

        String uri = request.getRequestURL().toString();
        timeLog.endStep("getUri");
        String url = uri;

        if (request.getQueryString() != null) {
            url += "?" + BizFilterUtil.urlSecurityEscape(request.getQueryString());
        }

        if (uri != null && !uri.contains("/Login.do")) {
            request.setAttribute("origURL", url);
        }
        timeLog.endStep("setOrigURL");

        // 将referer取出来，save到cookie中，使下一个请求能够知道这个请求的referer
        //        String referer = request.getHeader("Referer");
        //        if (referer != null) {
        ////            if (!referer.endsWith("ajaxproxy.htm")) {
        ////                CookieManager.getInstance().saveCookie(response, OpiConstants.BEFORE_REFERER,
        ////                        referer);
        ////            } else {
        ////                CookieManager.getInstance().saveCookie(response, OpiConstants.BEFORE_REFERER,
        ////                        getCookieReferer(request));
        ////            }
        //        }
//        CookieManager.getInstance().saveCookie(response, OpiConstants.BEFORE_REFERER,
//                getCookieReferer(request));
//        CookieManager.getInstance().saveCookie(response, OpiConstants.REFERER, url);

        //

        User host = BaseThreadUtil.currentHost(request);
        if (host == null) {
            Integer hostId = null;
            try {
                hostId = PassportManager.getInstance().getHostIdFromCookie(request, response);
            } catch (Ice.TimeoutException e) {
                e.printStackTrace();
                RootLogger.error("", e);
            }
            if (hostId != null) {
                try {
                    host = WUserAdapter.getInstance().get(hostId);
                    BaseThreadUtil.setHost(request, host);
                } catch (Ice.TimeoutException e) {
                    RootLogger.error("", e);
                }
            }
        }
        timeLog.endStep("GetUser");

        try {
            // 用户行为追踪
            BizFilterUtil.trackActivity(request);
            //为WebPager添加一个cookie来冒充session id
            BizFilterUtil.addCookieForIM(request, response);
            timeLog.endStep("BizFilterUtil.trackActivity");

            if (host != null) {
            	//因为糯米的进入 逻辑变更成模10
                if (host.getStatus() % 10 >= User.statusActivateVerify) {
                    response.sendRedirect(OpiConstants.urlLogin + "/Logout.do");
                    timeLog.endFinally();
                    return null;
                }
            }

            // 安全中心认证

            if (host != null && (host.getSafeStatus()&(31<<1)) > 0
                    && !SafeCenterFilter.isSafeCenterDisabled()) {
                SafeCenterHandler.doSafeCenter(response);
                return null;
            }

            request.setAttribute("host", BaseThreadUtil.currentHost(request));
            request.setAttribute("visiter", BaseThreadUtil.currentHost(request));
            timeLog.endStep("setAttributeHost");
            ActionForward af = executeR(mapping, form, request, response);
            timeLog.endStep("executeR end");
            timeLog.endFinally();
            return af;

        } catch (Exception e) {
            System.err.println("URL: " + request.getRequestURL());
            e.printStackTrace(System.err);
            timeLog.endException(e);
            return mapping.findForward(OpiConstants.forwardError);
        } finally {
            try {
                BaseThreadUtil.clearHost(request);
            } catch (Exception e1) {
                e1.printStackTrace();
            }
            timeLog.endFinally();
        }

    }

}
