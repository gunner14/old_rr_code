package com.xiaonei.passport.web.remoteip;

import java.util.Enumeration;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.base.wrapper.NativeIpWhiteList;
import com.xiaonei.platform.core.opt.base.wrapper.RemoteAddrFether;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class RemoteAddrFetherImpl implements RemoteAddrFether {

    /**
     * 获取ClientIp的Log
     */
    private final static Log logger = LogFactory.getLog(RemoteAddrFetherImpl.class);

    private static NativeIpWhiteList nativeIps = NativeIpWhiteList.getInstance();

    private static final Pattern ipPattern = Pattern.compile("([0-9]{1,3}\\.){3}[0-9]{1,3}");

    /**
     * 取客户端的真实IP，考虑了反向代理等因素的干扰
     * 
     * @param request
     * @return
     */
    @Override
    public String getRemoteAddr(final HttpServletRequest request) {
        if (logger.isDebugEnabled()) {// 打印所有日志
            logger.debug(new StringBuffer().append("X-Forwarded-For:")
                    .append(request.getHeader("X-Forwarded-For")).append("\tProxy-Client-IP:")
                    .append(request.getHeader("Proxy-Client-IP")).append("\t:WL-Proxy-Client-IP:")
                    .append(request.getHeader("WL-Proxy-Client-IP")).append("\tRemoteAddr:")
                    .append(request.getRemoteAddr()).toString());
        }
        String ip;
        @SuppressWarnings("unchecked")
        final Enumeration<String> xffs = request.getHeaders("X-Forwarded-For");
        if (xffs.hasMoreElements()) {
            final String xff = xffs.nextElement();
            ip = this.resolveClientIPFromXFF(xff);
            if (this.isValidIP(ip)) {
                if (logger.isInfoEnabled()) {
                    logger.info("X-Forwarded-For" + ip);
                }
                return ip;
            }
        }
        ip = request.getHeader("Proxy-Client-IP");
        if (this.isValidIP(ip)) {
            if (logger.isInfoEnabled()) {
                logger.info("Proxy-Client-IP" + ip);
            }
            return ip;
        }
        ip = request.getHeader("WL-Proxy-Client-IP");
        if (this.isValidIP(ip)) {
            if (logger.isInfoEnabled()) {
                logger.info("WL-Proxy-Client-IP" + ip);
            }
            return ip;
        }
        if (logger.isInfoEnabled()) {
            logger.info("None-Proxy-Client-IP" + ip);
        }
        return request.getRemoteAddr();
    }

    /**
     * 是否公司内部IP
     * 
     * @param ip
     * @return
     */
    private boolean isNativeIP(final String ip) {
        final int index = ip.lastIndexOf('.');
        final String ipPattern = ip.substring(0, index) + ".*";
        return nativeIps.isNativeIp(ipPattern);
    }

    private boolean isValidIP(final String ip) {
        if ((ip == null) || (ip.length() == 0) || "unknown".equalsIgnoreCase(ip)) {
            return false;
        }
        return ipPattern.matcher(ip).matches();
    }

    /**
     * 从X-Forwarded-For头部中获取客户端的真实IP。
     * X-Forwarded-For并不是RFC定义的标准HTTP请求Header
     * ，可以参考http://en.wikipedia.org/wiki/X-Forwarded-For
     * 
     * @param xff X-Forwarded-For头部的值
     * @return 如果能够解析到client IP，则返回表示该IP的字符串，否则返回null
     */
    private String resolveClientIPFromXFF(final String xff) {
        if ((xff == null) || (xff.length() == 0)) {
            return null;
        }
        final String[] ss = xff.split(",");
        for (int i = ss.length - 1; i >= 0; i--) {//x-forward-for链反向遍历
            final String ip = ss[i].trim();
            if (this.isValidIP(ip) && !this.isNativeIP(ip)) { //判断ip是否合法，是否是公司机房ip
                return ip;
            }
        }

        //如果反向遍历没有找到格式正确的外网IP，那就正向遍历找到第一个格式合法的IP
        for (int i = 0; i < ss.length; i++) {
            final String ip = ss[i].trim();
            if (this.isValidIP(ip)) {
                return ip;
            }
        }
        return null;
    }

}
