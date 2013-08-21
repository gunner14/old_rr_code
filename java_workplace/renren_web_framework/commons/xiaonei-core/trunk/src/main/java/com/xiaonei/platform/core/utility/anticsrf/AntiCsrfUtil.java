package com.xiaonei.platform.core.utility.anticsrf;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;

/**
 * 防CSRF的工具
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-1 下午06:17:38
 */
public class AntiCsrfUtil {
	
	private static Log logger = LogFactory.getLog(AntiCsrfUtil.class);
	
	/**
	 * 计算token时用的一个随机字符串，目的是不让用户猜到算法
	 */
	public static final String TOKEN_PREFIX = "843a3582";
	
	/**
	 * 根据指定ticket计算requestToken
	 * @param ticket
	 * @return
	 */
	public static String digestToken(String ticket) {
		return Integer.toHexString((TOKEN_PREFIX + ticket).hashCode());
	}
	
	/**
	 * 验证token
	 * 
	 * @param ticket
	 * @param token
	 * @return
	 */
	public static boolean validateToken(String ticket, String token) {
		String expectedToken = digestToken(ticket);
		return expectedToken.equals(token);
	}

	/**
	 * 取出requestToken的值，检查token是否合法。
	 * 
	 */
	public static boolean validateToken(HttpServletRequest request, String tokenParamName) {

		String requestToken = request.getParameter(tokenParamName);
		
		if (logger.isDebugEnabled()) {
			logger.debug("requestToken to validate: " + requestToken);
		}

		if (requestToken == null) {
			return false;
		}
		
		String ticket = CookieManager.getInstance().getCookie(request,
				PassportManager.loginKey_ticket);

		if (ticket == null) {
			//没有票的情况就用客户端地址
			ticket = request.getRemoteAddr();
			if (logger.isDebugEnabled()) {
				logger.debug("use remote adresss to validate token: " + ticket);
			}
		} else {
			if (logger.isDebugEnabled()) {
				logger.debug("use ticket to validate token: " + ticket);
			}
		}

		return AntiCsrfUtil.validateToken(ticket, requestToken);
	}

	public static String generateToken(HttpServletRequest request) {
		
		// 从cookie中取t票
		String ticket = CookieManager.getInstance().getCookie(request,
				PassportManager.loginKey_ticket);
		if (ticket == null) {
			//没有票的情况就用客户端地址
			ticket = request.getRemoteAddr();
			if (logger.isDebugEnabled()) {
				logger.debug("use remote adresss to digest token: " + ticket);
			}
		} else {
			if (logger.isDebugEnabled()) {
				logger.debug("use ticket to digest token: " + ticket);
			}
		}
		return AntiCsrfUtil.digestToken(ticket);
	}
}
