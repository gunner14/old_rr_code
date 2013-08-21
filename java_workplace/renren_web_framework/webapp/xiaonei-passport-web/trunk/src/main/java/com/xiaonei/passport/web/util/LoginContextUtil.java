package com.xiaonei.passport.web.util;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.renren.passport.service.LoginContext;
import com.xiaonei.passport.web.constants.OrigUrlDefaultDef;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.remoteip.RemoteAddrFetherImpl;
/**
 * 获取和登陆相关的参数
 * @author Administrator
 *
 */
public class LoginContextUtil {
	final static RemoteAddrFetherImpl ADDR_FETHER_IMPL = new RemoteAddrFetherImpl();
	/**
	 * 获取登陆上下文
	 * @param request
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public static LoginContext getLoginContext(HttpServletRequest request){
		LoginContext context=new LoginContext();
		context.setHost(request.getRemoteHost());
		context.setUserAgent( request.getHeader("User-Agent"));
		context.setCooikes(getCookies(request));
		context.setReferer(request.getHeader("Referer"));
		String ip=getClientIp(request);
		context.setIp(ip);
		context.setRequestUrl(request.getRequestURI());
		return context;
	}
	
	/**
	 * 获取客户端ip
	 * @param request
	 * @return
	 */
	public static String getClientIp(HttpServletRequest request){
		 final String ip = ADDR_FETHER_IMPL.getRemoteAddr(request);
		 return ip;
	}
	
	/**
     * 获取errorUrl
     * @param domain
     * @param origUrl
     * @return
     */
    public static String getErrorUrl(String errorUrl, String defaultUrl) {
        if(StringUtils.isEmpty(errorUrl)){
            errorUrl = defaultUrl;
        } else {
            errorUrl = getOrigUrl(errorUrl);
        }
        return errorUrl;
    }
    
	
	/**
	 * 获取origUrl
	 * @param domain
	 * @param origUrl
	 * @return
	 */
	public static String getOrigUrl(String origUrl) {
		if (StringUtils.isEmpty(origUrl)) {
			origUrl = OrigUrlDefaultDef.DEFAULT_ORIG_URL;
		}
		// 如果不能构造成url对象,则认为这是一个url encode 的结果,需要decode
		try {
			new URL(origUrl);
		} catch (final MalformedURLException e1) {
			try {
				origUrl = URLDecoder.decode(origUrl, "UTF-8");
			} catch (final UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}
		if (origUrl.toLowerCase().contains(OrigUrlDefaultDef.DEFAULT_LOGIN_URL)) {
			origUrl = OrigUrlDefaultDef.DEFAULT_ORIG_URL;
		}
		
		if(StringUtils.startsWith(origUrl, "http://zhan.renren.com/home")) {
            return origUrl.replaceAll("&amp;", "&");
        }
		return origUrl;
	}
	
	public static String getUrl(String origUrl, String defaultUrl) {
		if(StringUtils.isEmpty(origUrl)) {
			return defaultUrl;
		}
		return origUrl;
	}

	public static  String getOrigUrlEncode( final String origUrl) {
		try {
			return URLEncoder.encode(getOrigUrl( origUrl), "UTF-8");
		} catch (final UnsupportedEncodingException e) {
			e.printStackTrace();
			return "";
		}
	}
	
	
	
	/**
	 * 获取cooike信息
	 * @param request
	 * @return
	 */
	private static Map<String, String> getCookies(HttpServletRequest request){
		Map<String, String> cookieMap=new HashMap<String, String>();
		try{
			Cookie[] cookies = request.getCookies();
			if (cookies == null || cookies.length == 0){
				return cookieMap;
			}
			for (int i = 0; i < cookies.length; i++) {
				Cookie cookie=cookies[i]; 
				cookieMap.put(cookie.getName(), cookie.getValue());
			}
		}catch(Exception e){
			e.printStackTrace();
			return cookieMap;
		}
				
		return cookieMap;
	}
}
