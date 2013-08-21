package com.xiaonei.platform.core.opt.base.action;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Map;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.xce.log.TimeCost;

/**
 * 
 * BaseAction.java
 * 
 * @author drizzt(drizzt.wang@gmail.com) 2006-1-31 17:18:04
 */
public abstract class BaseAction extends BaseXnAction {

    public abstract ActionForward executeBt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws Exception;

    private final Log BaseLogger = LogFactory.getLog("core");

    protected int newMessageCount = 0;

    protected String HOST_USER_CONFIG_INFO = "HOST_USER_CONFIG_INFO";

    @Override
    public ActionForward executeXn(final ActionMapping mapping, final ActionForm form,
            final HttpServletRequest request, final HttpServletResponse response) {
        // For AD around the school
        TimeCost timeLog = TimeCost.begin(BaseLogger, "BaseAction");
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", 0);
        String servletPath = request.getServletPath();
        request.setAttribute("ServletPath", servletPath);
        String aadsIdString = request.getParameter("id");
        request.setAttribute("AroundADSchoolID", aadsIdString);
        try {
            User host = BaseThreadUtil.currentHost(request);
            if (host != null) {
                if ((true/*host.getStatus() < User.statusActivateVerified)
                        || (host.getStatus() == User.statusActivatedVerify)
                        || UserConfirmLogic.getInstance().isConfirmed(host)
                        || (OpiConstants.isKaixin() && host.getStatus() <= User.statusActivateVerified*/) // 开心不激活也要取UserConfig
                ) {
                    // 非登录/登出的情况通知中间层
                    boolean tt = isLoginOrLogout();
                    if (!tt) {
                        //TODO:把改逻辑放到登录和自动登录处
                        int targetUserId = host.getId();
                        // 请求更新状态前检查cookie中是否有此类信息
                        if (validate(targetUserId, request)) {
                            BaseAction.registerOnlineCookie(targetUserId, response);
                            timeLog.endStep("registerOnlineCookie");
                        }
                    }

                    try {
                        // 左侧的“导航”配置
                        UserConfigInfo config = WUserConfigInfoAdapter.getInstance().getUserConfig(
                                host.getId());
                        // 将登陆者的UserConfigInfo对象放到request中，不用每次都取，子类可以直接从request中获得
                        timeLog.endStep("getUserConfigInfo");
                        request.setAttribute(HOST_USER_CONFIG_INFO, config);

                        request.setAttribute("userMenu", UserMenuManager.getInstance().get(
                                host.getId()));
                        logger.warn("Deprecated web framework. The logic about menu is not supported any more.");

                        Map<Integer, Integer> userCountMap = UserCountMgr.getCountMap(host.getId());
                        if (userCountMap != null) { // 如果ICETimeout，这里就可能是null了
                            request.setAttribute("userCountMap", userCountMap);
                            request.setAttribute("newMessageCount", userCountMap
                                    .get(UserCountMgr.NEW_MESSAGE));
                            request.setAttribute("newNotifyCount", userCountMap
                                    .get(UserCountMgr.NOTIFY));
                        }

                        timeLog.endStep("getNewMsgCount");
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                } else {
                    host = null;
                    //					BaseThreadUtil.clearHost(request);
                    //					timeLog.endStep("clearHost");
                }
            }
            ActionForward af;
            try {
                timeLog.endStep("executeBt Start");
                af = executeBt(mapping, form, request, response);
                timeLog.endStep("executeBt end");
            } catch (SQLException e) {
                timeLog.endStep("executeBt Exception", e);
                e.printStackTrace();
                //TODO:wap是否还需要此段逻辑
                if (BaseAction.isFromWap(response, request)) {
                    BaseXnAction.addMsg(request, response, "错误的网址!");
                    return BaseAction.redirectWapHome(response);
                }
                return errUnknown(mapping, request);
            }
            timeLog.endStep("return ActionForward;");
            return af;
        } catch (Throwable t) {
            timeLog.endStep("Throwable found");
            t.printStackTrace();
            if (BaseAction.isFromWap(response, request)) {
                BaseXnAction.addMsg(request, response, "错误的网址!");
                return BaseAction.redirectWapHome(response);
            }
            return errUnknown(mapping, request);
        } finally {
            timeLog.endFinally();
        }
    }

    /**
     * add by lifs <br>
     * 判断是否是登录或登出，只有登录和登出的子类Action需要重写此方法，目的是避免多次通知中间层用户在线状态。<br>
     * 其他子类不需要重写此方法。
     * 
     * @return
     */
    protected final boolean isLoginOrLogout() {
        return false;
    }

    /**
     * 验证cookie中是否已经存在此用户的信息，cookie中有，则验证失败返回false<br>
     * add by lifs 2007-01-10
     * 
     * @param hostId
     * @param request
     * @return
     */
    private boolean validate(final int hostId, final HttpServletRequest request) {
        Cookie[] cookies = request.getCookies();
        if ((cookies == null) || (cookies.length == 0)) return true;
        if (isFindCookie(hostId, cookies)) return false;
        return true;
    }

    /**
     * add by lifs 2007-01-10<br>
     * 在传入的cookie集合中查找某一个cookie，找到返回true
     * 
     * @param hostId
     * @param cookies cookie的集合
     * @return
     */
    private boolean isFindCookie(final int hostId, final Cookie[] cookies) {
        String cookieName = "WebOnLineNotice_";
        for (Cookie element : cookies)
            if (element.getName().equals(cookieName + hostId)) return true;
        return false;
    }

    /**
     * add by lifs 2007-01-10<br>
     * 设置在线Cookie值
     * 
     * @param hostId
     * @param response
     * @return
     */
    protected static boolean registerOnlineCookie(final int hostId,
            final HttpServletResponse response) {

        CookieManager.getInstance().saveCookie(response, "WebOnLineNotice_" + hostId, "1", 5 * 60,
                "/");
        return true;
    }

    /**
     * add by lifs 2007-01-10<br>
     * 清除在线Cookie值
     * 
     * @param hostId
     * @param response
     * @return
     */
    protected static boolean clearOnlineCookie(final int hostId, final HttpServletResponse response) {

        CookieManager.getInstance().saveCookie(response, "WebOnLineNotice_" + hostId, null, 0, "/");
        return true;
    }

    /*
     * 判断是否 未激活已验证
     */
    public static boolean checkUserActivateVerified(final User host) {
        if (host.getStatus() == User.statusActivateVerified) return true;
        return false;
    }

    /*
     * 判断是否 已激活已验证
     */
    public static boolean checkUserActivatedVerified(final User host) {
        if (host.getStatus() < User.statusActivateVerified) return true;
        return false;
    }

    public ActionForward noPermission(final ActionMapping mapping,
            final HttpServletRequest request, final String msg) {
        BaseXnAction.addMsg(request, msg);
        return mapping.findForward(BaseXnAction.noPermission);
    }

    public ActionForward noPermission(final ActionMapping mapping, final HttpSession session,
            final String msg) {
        BaseXnAction.addMsg(session, msg);
        return mapping.findForward(BaseXnAction.noPermission);
    }

    public ActionForward errUrl(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session) {
        BaseXnAction.addErr(session, BaseXnAction.errUrl);
        logUrlError(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errUrl(final HttpServletRequest request, final ActionMapping mapping,
            final String msg) {
        BaseXnAction.addErr(request, msg);
        logUrlError(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errUnknown(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session) {
        BaseXnAction.addErr(session, BaseXnAction.errUnknown);
        logErr(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errDB(final HttpServletRequest request, final ActionMapping mapping,
            final String msg) {
        BaseXnAction.addErr(request, msg);
        logErr(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errOther(final HttpServletRequest request, final ActionMapping mapping,
            final String msg) {
        BaseXnAction.addErr(request, msg);
        logErr(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errOther(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session, final String msg) {
        BaseXnAction.addErr(session, msg);
        logErr(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    public ActionForward errNotFound(final HttpServletRequest request, final ActionMapping mapping,
            final String msg) {
        BaseXnAction.addErr(request, msg);
        logNotFound(request);
        return mapping.findForward("errornotexist");
    }

    public ActionForward errNotFound(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session, final String msg) {
        BaseXnAction.addErr(session, msg);
        logNotFound(request);
        return mapping.findForward("errornotexist");
    }

    public ActionForward errNotFound(final HttpServletRequest request, final ActionMapping mapping) {

        logNotFound(request);
        return mapping.findForward("errornotexist");
    }

    public ActionForward errUpload(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session, final String msg) {
        BaseXnAction.addErr(session, msg);
        logNotFound(request);
        return mapping.findForward("errorupload");
    }

    public ActionForward errRegister(final HttpServletRequest request, final ActionMapping mapping,
            final HttpSession session, final String msg) {
        BaseXnAction.addErr(session, msg);
        logNotFound(request);
        return mapping.findForward("errorregister");
    }

    public static String getFromIp(final HttpServletRequest request) {
        return BizFilterUtil.getClientIP(request);
    }

    @Override
    public boolean isPost(final HttpServletRequest request) {
        if (request.getMethod().toLowerCase().equals("post")) return true;
        return false;
    }

    public static boolean isFromWap(final HttpServletResponse response,
            final HttpServletRequest request) {
        boolean t = false;
        String serverName = request.getServerName();
        String referer = request.getHeader("referer");

        try {
            t = ((serverName == null ? false : serverName
                    .equalsIgnoreCase(OpiConstants.domainMobile.toString()))
                    || (serverName == null ? false : serverName.contains("xiaonei.cn"))
                    || (referer == null ? false : referer.contains(OpiConstants.urlMobile
                            .toString())) || (referer == null ? false : referer
                    .contains("xiaonei.cn")));
        } catch (RuntimeException e) {
            e.printStackTrace();
        }

        return t;
    }

    public static ActionForward redirectWapHome(final HttpServletResponse response) {
        try {
            response.sendRedirect(OpiConstants.urlMobile + "/whome.do");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static ActionForward redirectWapLogin(final HttpServletResponse response) {
        try {
            response.sendRedirect(OpiConstants.urlMobile + "/wlogin.do");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 从request得到登陆者的UserConfigInfo对象
     * 
     * @param request
     * @return
     */
    protected UserConfigInfo getHostUserConfigInfo(final HttpServletRequest request) {
        Object obj = request.getAttribute(HOST_USER_CONFIG_INFO);
        if (obj != null) return (UserConfigInfo) obj;
        return null;
    }

    /**
     * 下面增加一组处理错误消息的函数，目的是为了把原来的HttpSession参数去掉
     */

    /*
     * public ActionForward noPermission(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response, final
     * String msg) { BaseXnAction.addMsg(request, response, msg); return
     * mapping.findForward(BaseXnAction.noPermission); }
     * 
     * public ActionForward errUrl(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response) {
     * BaseXnAction.addErr(request, response, BaseXnAction.errUrl);
     * logUrlError(request); return mapping.findForward(BaseXnAction.toError); }
     */

    public ActionForward errUrl(final ActionMapping mapping, final HttpServletRequest request) {
        return errUrl(request, mapping, BaseXnAction.errUrl);
    }

    /*
     * public ActionForward errUnknown(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response) {
     * BaseXnAction.addErr(request, response, BaseXnAction.errUnknown);
     * logErr(request); return mapping.findForward(BaseXnAction.toError); }
     */

    public ActionForward errUnknown(final ActionMapping mapping, final HttpServletRequest request) {
        BaseXnAction.addErr(request, BaseXnAction.errUnknown);
        logErr(request);
        return mapping.findForward(BaseXnAction.toError);
    }

    /*
     * public ActionForward errOther(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response, final
     * String msg) { BaseXnAction.addErr(request, response, msg);
     * logErr(request); return mapping.findForward(BaseXnAction.toError); }
     * 
     * public ActionForward errNotFound(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response, final
     * String msg) { BaseXnAction.addErr(request, response, msg);
     * logNotFound(request); return mapping.findForward("errornotexist"); }
     * 
     * public ActionForward errUpload(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response, final
     * String msg) { BaseXnAction.addErr(request, response, msg);
     * logNotFound(request); return mapping.findForward("errorupload"); }
     * 
     * public ActionForward errRegister(final ActionMapping mapping, final
     * HttpServletRequest request, final HttpServletResponse response, final
     * String msg) { BaseXnAction.addErr(request, response, msg);
     * logNotFound(request); return mapping.findForward("errorregister"); }
     */
}
