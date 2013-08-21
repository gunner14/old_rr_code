package com.xiaonei.login.logic.status;

import javax.servlet.http.HttpServletRequest;

public interface PassportMessage {
	/**
	 * addErrorMessage 添加错误提示信息
	 * 
	 * @param request
	 * @param message
	 * 
	 * @author tai.wang@opi-corp.com Dec 18, 2009 - 12:05:21 PM
	 */
	public void addErrorMessage(HttpServletRequest request, String message);

	/**
	 * addWarningMessage 添加警告提示信息
	 * 
	 * @param request
	 * @param message
	 * 
	 * @author tai.wang@opi-corp.com Dec 18, 2009 - 12:05:37 PM
	 */
	public void addWarningMessage(HttpServletRequest request, String message);
}
