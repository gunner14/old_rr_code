package com.renren.sns.wiki.utils;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFetherImpl;

/**
 * 用户获取客户端的IP
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-24
 * 
 */
public class RemoteAddrUtil {

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

}
