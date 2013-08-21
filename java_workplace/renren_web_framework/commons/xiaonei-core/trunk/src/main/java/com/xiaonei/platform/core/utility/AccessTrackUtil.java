package com.xiaonei.platform.core.utility;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFetherImpl;
import com.xiaonei.xce.log.ActiveTrack;
import com.xiaonei.xce.log.CombinedLog;

/**
 * 跟access track相关的工具类，原来被封装在BizFilterUtil中，因为整理了依赖关系，
 * 所以单提出来了。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-2 上午11:44:00
 */
public class AccessTrackUtil {

    private static final Log logger = LogFactory.getLog(AccessTrackUtil.class);


    // 用于正确获取请求IP，特别是在代理转发的情况下
    private static final RemoteAddrFether remoteAddrFether = new RemoteAddrFetherImpl();
	
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
        trackActivity(request, null);
    }

    /**
     * 用户行为追踪,从当前请求中取host
     * 
     * @param request 当前请求
     * @param decision 由业务逻辑决定的标志。
     */
    public static void trackActivity(HttpServletRequest request, String decision) {
        try {
            User host = BaseThreadUtil.currentHost(request);
            int hostid = 0;
            if (host != null) {
                hostid = host.getId();
            }
            trackActivity(hostid, request, decision);
        } catch (Throwable e) {
            System.err.println("Error occurs while tracking user activity: " + e.getMessage());
        }
    }

    /**
     * 用户行为追踪,从当前请求中取host
     * 
     * @param anonymIdent 用户唯一匿名id
     * @param request 当前请求
     * @param decision 由业务逻辑决定的标志。
     */
    public static void trackActivity(String anonymIdent, HttpServletRequest request, String decision) {
        try {
            User host = BaseThreadUtil.currentHost(request);
            int hostid = 0;
            if (host != null) {
                hostid = host.getId();
            }
            trackActivity(anonymIdent, hostid, request, decision);
        } catch (Throwable e) {
            System.err.println("Error occurs while tracking user activity: " + e.getMessage());
        }
    }

    /**
     * 用户行为追踪
     * 
     * @param hostid 用户id
     * @param request 当前请求
     */
    public static void trackActivity(Integer hostid, HttpServletRequest request) {
        trackActivity(hostid, request, null);
    }

    public static void trackActivity(Integer hostid, HttpServletRequest request, String decision) {
      trackActivity(null, hostid, request, decision);
    }

    /**
     * 用户行为追踪
     * 
     * @param anonymIdent 用户唯一匿名id
     * @param hostid 用户id
     * @param request 当前请求
     * @param decision 由业务逻辑决定的标志。
     */
    public static void trackActivity(String anonymIdent, Integer hostid, HttpServletRequest request, String decision) {
        try {
            String url = getUrlWithQueryString(request);
            //用户行为追踪
            String ip = getClientIP(request);
            int uid = 0;
            if (hostid != null) { //用户未登陆就传0
                uid = hostid;
            }
            if (logger.isDebugEnabled()) {
                logger.debug("ActiveTrack.access(" + anonymIdent + "," + uid + "," + "ip," + url + ");");
            }
            
            ActiveTrack.access(anonymIdent, uid, ip, url, decision);
            String referer = request.getHeader("Referer");
            String userAgent = request.getHeader("User-Agent");
			CombinedLog log = new CombinedLog(ip, "-", uid, System
					.currentTimeMillis(), url, -1, -1, referer, userAgent);
            ActiveTrack.combined(uid, log);
        } catch (Throwable e) {
            logger.warn("Error occurs while tracking user activity:", e);
        }
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

	
}
