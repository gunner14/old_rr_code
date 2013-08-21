package com.dodoyo.invite.core;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.opt.base.action.BaseAction;


public class InviteUtil {
    /**
     * 用于通过Request得到IP地址
     *
     * @param request
     * @return
     */
    public static String getIpAddress(HttpServletRequest request) 
    {
    	try
    	{
    		return BaseAction.getFromIp(request);
    	}
    	catch(Exception e)
    	{
    		return getIpAddressBak(request);
    	}
    	
	}
    
    private static String getIpAddressBak(HttpServletRequest request)
    {
      String ip = request.getHeader("x-forwarded-for");
      if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
          ip = request.getHeader("Proxy-Client-IP");
      }
      if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
          ip = request.getHeader("WL-Proxy-Client-IP");
      }
      if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
          ip = request.getRemoteAddr();
      }
      return ip;
    }
}
