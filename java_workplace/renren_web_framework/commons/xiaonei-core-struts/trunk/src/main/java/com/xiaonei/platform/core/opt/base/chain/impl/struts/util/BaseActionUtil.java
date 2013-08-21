package com.xiaonei.platform.core.opt.base.chain.impl.struts.util;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.sysmsg.SysMsgManager;

public class BaseActionUtil {
	
	/**
	 * 取客户端的真实IP，考虑了反向代理等因素的干扰
	 * 
	 * @param request
	 * @return
	 */
	public static String getClientIP(final HttpServletRequest request) {
		return BizFilterUtil.getClientIP(request);
	}

	/**
	 * 向请求中添加message
	 * 
	 * @param request
	 * @param msg
	 */
	public static void addMsg(HttpServletRequest request, final String msg) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", msg);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, ams);
	}

	/**
	 * 向session中添加message；session使用memcached来实现
	 * 
	 * @param request
	 * @param response
	 * @param msg
	 */
	public static void addMsg(HttpServletRequest request, HttpServletResponse response, String msg) {
		SysMsgManager.getInstance().adSysMsg(request, response, msg, SysMsgManager.MSG_TYPE_WARNING);
	}

	/**
	 * 向请求中添加error message
	 * 
	 * @param request
	 * @param err
	 */
	public static void addErr(HttpServletRequest request, final String err) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", err);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.ERROR_KEY, ams);
	}

	/**
	 * 添加session error消息；session使用memcached来实现
	 * 
	 * @param msg
	 */
	public static void addErr(HttpServletRequest request, HttpServletResponse response, String msg) {
		SysMsgManager.getInstance().adSysMsg(request, response, msg);
	}
}
