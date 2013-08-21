package com.xiaonei.platform.core.opt.base.chain.impl;

import java.util.UUID;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFetherImpl;
import com.xiaonei.platform.core.sysmsg.SysMsgManager;
import com.xiaonei.platform.core.utility.AccessTrackUtil;

/**
 * 
 * 这个类将原来Action继承体系中基类的一些工具方法提取了出来
 * 
 * @author Li Weibo
 * @since 2008-12-30
 */
public class BizFilterUtil {

    private static final Logger chainLogger = Logger.getLogger("chain_logger");

    private static final Log chainTimeCostLog = LogFactory.getLog("chain_timecost");

    // 用于正确获取请求IP，特别是在代理转发的情况下
    private static final RemoteAddrFether remoteAddrFether = new RemoteAddrFetherImpl();

    /**
     * 通过这个方法来统一控制整个chain框架中使用的logger
     * 
     * @return
     */
    public static Logger getLogger() {
        return chainLogger;
    }

    public static Log getTimeCostLog() {
        return chainTimeCostLog;
    }

    /**
     * 判断当前请求是否来自于wap
     * 
     * @param response
     * @param request
     * @return
     */
    public static boolean isFromWap(final HttpServletRequest request,
            final HttpServletResponse response) {

        String serverName = request.getServerName();
        String referer = request.getHeader("Referer");

        if (serverName != null) {
            if (serverName.endsWith(OpiConstants.domainMobile.toString())
                    || serverName.endsWith(OpiConstants.domainMobileApps.toString())
                    || serverName.contains("xiaonei.cn")) {
                return true;
            }
        }

        if (referer != null) {
            if (referer.startsWith(OpiConstants.urlMobile.toString())
                    || referer.startsWith(OpiConstants.urlMobileApps.toString())
                    || referer.contains("xiaonei.cn")) {
                return true;
            }
        }
        return false;
    }

    /**
     * 取客户端的真实IP，考虑了反向代理等因素的干扰
     * 
     * @param request
     * @return
     */
    public static String getClientIP(final HttpServletRequest request) {
        return remoteAddrFether.getRemoteAddr(request);
    }

    /**
     * 用户行为追踪,从当前请求中取host
     * 
     * @param request 当前请求
     */
    public static void trackActivity(HttpServletRequest request) {
        AccessTrackUtil.trackActivity(request, null);
    }

    /**
     * 用户行为追踪,从当前请求中取host
     * 
     * @param request 当前请求
     * @param decision 由业务逻辑决定的标志。
     */
    public static void trackActivity(HttpServletRequest request, String decision) {
    	AccessTrackUtil.trackActivity(request, decision);
    }

    /**
     * 用户行为追踪
     * 
     * @param hostid 用户id
     * @param request 当前请求
     */
    public static void trackActivity(Integer hostid, HttpServletRequest request) {
    	AccessTrackUtil.trackActivity(hostid, request, null);
    }

    /**
     * 用户行为追踪
     * 
     * @param hostid 用户id
     * @param request 当前请求
     * @param decision 由业务逻辑决定的标志。
     */
    public static void trackActivity(Integer hostid, HttpServletRequest request, String decision) {
    	AccessTrackUtil.trackActivity(hostid, request, decision);
    }

    /**
     * 取得当前请求的带queryString的url
     * 
     * @param request
     * @return
     */
    public static String getUrlWithQueryString(HttpServletRequest request) {
        StringBuffer url = request.getRequestURL();
        int len = url.length();
        String queryString = request.getQueryString();
        String _resourceId = (String) request.getAttribute("_resourceId");
        if (_resourceId == null) {
            _resourceId = request.getRequestURI();
        }
        if (queryString != null && queryString.length() > 0) {
            url.append('?');
            url.append(queryString);
        }
        if (url.length() == len) {
            url.append('?');
        } else {
            url.append('&');
        }
        url.append("_resourceId=").append(_resourceId);
        return url.toString();
    }

    /**
     * 将url中的危险字符转义
     * 
     * @param url
     * @return
     */
    public static String urlSecurityEscape(String url) {
        if (url == null) {
            return null;
        }
        StringBuilder sb = new StringBuilder(url.length());
        for (int i = 0; i < url.length(); i++) {
            char c = url.charAt(i);
            if (c == '<') {
                sb.append("&lt;");
            } else if (c == '>') {
                sb.append("&gt;");
            } else if (c == '"') {
                sb.append("&quot;");
            } else if (c == '\'') {
                //这个有风险，所以滤掉
            } else {
                sb.append(c);
            }
        }
        return sb.toString();
    }

    private static final String XNE_SSESSION_ID = "XNESSESSIONID";

    /**
     * 关掉resin的session之后，cookie里不再有XNESSESSIONID了，但是webpapger还需要这个东西，所以这里再
     * 随机产生一个放到cookie里
     * 
     * @param request
     * @param response
     */
    public static void addCookieForIM(HttpServletRequest request, HttpServletResponse response) {
        try {
            String old = CookieManager.getInstance().getCookie(request, XNE_SSESSION_ID);
            if (old == null) {
                Cookie cookie = new Cookie(XNE_SSESSION_ID, randomSmallUUID());
                cookie.setDomain("." + OpiConstants.domain);
                cookie.setPath("/");
                response.addCookie(cookie);
            }
        } catch (Exception e) {
            System.err.println("Error while add cookie " + XNE_SSESSION_ID + ":"
                    + e.getClass().getName() + " : " + e.getMessage());
        }
    }

    private static String randomSmallUUID() {
        UUID uuid = UUID.randomUUID();
        String[] ret = uuid.toString().split("-");
        return ret[ret.length - 1];
    }

    /**
     * 添加session message消息；session使用memcache来实现
     * 
     * @param msg
     */
    public static void addMsg(HttpServletRequest request, HttpServletResponse response, String msg) {
        SysMsgManager.getInstance()
                .adSysMsg(request, response, msg, SysMsgManager.MSG_TYPE_WARNING);
    }

    /**
     * 添加session error消息；session使用memcache来实现
     * 
     * @param msg
     */
    public static void addErr(HttpServletRequest request, HttpServletResponse response, String msg) {
        SysMsgManager.getInstance().adSysMsg(request, response, msg);
    }

    public static void addMsg(HttpServletRequest request, final String msg) {
        ActionMessages ams = new ActionMessages();
        ActionMessage am = new ActionMessage("errors.detail", msg);
        ams.add(ActionMessages.GLOBAL_MESSAGE, am);
        request.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, ams);
    }

    public static void addErr(HttpServletRequest request, final String err) {
        ActionMessages ams = new ActionMessages();
        ActionMessage am = new ActionMessage("errors.detail", err);
        ams.add(ActionMessages.GLOBAL_MESSAGE, am);
        request.setAttribute(org.apache.struts.Globals.ERROR_KEY, ams);
    }

}
