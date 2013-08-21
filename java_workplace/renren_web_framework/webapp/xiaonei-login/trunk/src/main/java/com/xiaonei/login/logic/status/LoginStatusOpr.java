package com.xiaonei.login.logic.status;

import com.xiaonei.login.constants.LoginStatusCode;

public interface LoginStatusOpr {
	/**
	 * 根据状态码返回对应的状态码描述信息
	 * 
	 * @param status
	 * @return
	 */
	public String getErrorMessage(LoginStatusCode status);
}
