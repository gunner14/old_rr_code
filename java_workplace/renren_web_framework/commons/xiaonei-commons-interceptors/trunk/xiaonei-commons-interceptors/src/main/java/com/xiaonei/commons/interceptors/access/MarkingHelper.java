package com.xiaonei.commons.interceptors.access;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.interceptors.MD5;
import com.xiaonei.commons.interceptors.access.annotation.MarkAsFromWap;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 作为统一接口判断一个请求来自什么客户端
 * 
 * @author zhiliang.wang
 * 
 */
public class MarkingHelper {

    protected static Log logger = LogFactory.getLog(MarkingHelper.class);

    static void forceMarkWapRequest(Invocation inv) {
        inv.addModel("isFromWap", Boolean.TRUE);
    }

    static void forceMarkImRequest(Invocation inv) {
        inv.addModel("isFromIM", Boolean.TRUE);
    }

    public static boolean isFromWapAndMark(Invocation inv) {
        Boolean isFromWap = (Boolean) inv.getModel().get("isFromWap");
        if (isFromWap != null) {
            return isFromWap;
        }
        MarkAsFromWap markAsFromWap = inv.getMethod().getAnnotation(MarkAsFromWap.class);
        if (markAsFromWap == null) {
            markAsFromWap = inv.getControllerClass().getAnnotation(MarkAsFromWap.class);
        }
        if (markAsFromWap != null) {
            return markAsFromWap.value();
        } else {
            HttpServletRequest request = inv.getRequest();
            String serverName = request.getServerName();
            if ("xiaonei.cn".equalsIgnoreCase(serverName)
                    || OpiConstants.domainMobile.toString().equalsIgnoreCase(serverName)) {
                return true;
            } else {
                String referer = request.getHeader("referer");
                if ("xiaonei.cn".equalsIgnoreCase(referer)
                        || OpiConstants.urlMobile.toString().equalsIgnoreCase(referer)) {
                    return true;
                }
            }
        }
        return false;
    }

    public static boolean isFromIMAndMark(Invocation inv, String ticket) {
        Boolean isFromIM = (Boolean) inv.getModel().get("isFromIM");
        String xeferer = null;
        // by http head
        if (isFromIM == null && ticket != null) {
            HttpServletRequest request = inv.getRequest();
            xeferer = request.getHeader("Xeferer");
            if (logger.isDebugEnabled()) {
                logger.debug("httpHeader.Xeferer=" + xeferer + "; ticket=" + ticket);
            }
            isFromIM = isValid(inv, ticket, xeferer);
        }
        // by http cookie
        if (isFromIM == null && ticket != null) {
            HttpServletRequest request = inv.getRequest();
            xeferer = getCookie(request, "Xeferer");
            if (logger.isDebugEnabled()) {
                logger.debug("httpCookie.Xeferer=" + xeferer + "; ticket=" + ticket);
            }
            isFromIM = isValid(inv, ticket, xeferer);
        }
        if (isFromIM == null) {
            isFromIM = Boolean.FALSE;
            if (logger.isDebugEnabled()) {
                logger.debug("xeferer is null or illegel: xeferer=" + xeferer);
            }
        }
        inv.addModel("isFromIM", isFromIM);
        return isFromIM;
    }

    protected static Boolean isValid(Invocation inv, String ticket, String xeferer) {
        if (xeferer != null && xeferer.length() == 16) {
            // "1e4548100fa84a18b070b4b628e323e1"此常量来自黄欢
            String result = MD5.digest("1e4548100fa84a18b070b4b628e323e1" // NL
                    + ticket + '\0').substring(0, 16); //后面加'\0'是为了兼容IM的一个bug
            boolean valid = xeferer.equals(result);
            if (logger.isDebugEnabled()) {
                logger.debug("expected Xeferer result: " //
                        + result + "   " + (valid ? "ok" : "fail"));
            }
            if (valid) {
                return Boolean.TRUE;
            }
        }
        // 返回null代表不做出肯定或否定判断
        return null;
    }

    private static String getCookie(HttpServletRequest request, String key) {
        Cookie[] cookies = request.getCookies();
        if (cookies == null || cookies.length == 0) {
            return null;
        }
        for (int i = 0; i < cookies.length; i++) {
            if (cookies[i].getName().equals(key)) {
                return cookies[i].getValue();
            }

        }
        return null;
    }

}
