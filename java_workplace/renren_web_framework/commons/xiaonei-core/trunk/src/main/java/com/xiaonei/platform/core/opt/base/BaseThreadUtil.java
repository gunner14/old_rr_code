package com.xiaonei.platform.core.opt.base;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

public class BaseThreadUtil {
	//private static final ThreadLocal<User> _host = new ThreadLocal<User>();
	private static final String _host="_curUser" ;
	public static void setHost(HttpServletRequest request,User host) {
		request.setAttribute(_host,host) ;
		try
		{
			if (host != null)
				request.setAttribute(OpiConstants.loginKey_session, host.getId());
		}catch(Exception e){}
	}

	public static void clearHost(HttpServletRequest request) {
		request.removeAttribute(_host) ;
	}
	public static User currentHost(HttpServletRequest request) {
		return (User)request.getAttribute(_host) ;
	}
	public static HttpSession currentHttpSession(HttpServletRequest request){
		return request.getSession() ;
	}

	/**
	 * 把当前用户的票放在request中，某些场景可能会用到这个票，如wap会把票缀在url后面
	 * @param request
	 * @param ticket
	 */
	public static void setTicket(HttpServletRequest request, String ticket) {
		request.setAttribute(PassportManager.loginKey_ticket, ticket);
	}
	
	/**
	 * 从request对象中取之前放进去的票。
	 * 注意，这不是验票方法
	 * @param request
	 * @return
	 */
	public static String currentTicket(HttpServletRequest request) {
		return (String)request.getAttribute(PassportManager.loginKey_ticket);
	}
	
	public static boolean beginJDBCTranx() throws SQLException {
		return true ;
	}
	
	public static boolean commitJDBCTranx() throws SQLException {
		return true ;
	}
	
	public static boolean rollbackJDBCTranx() {
		return true ;
	}	
}
