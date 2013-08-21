package com.xiaonei.commons.interceptors.access.anticsrf;

import javax.servlet.http.HttpServletRequest;

/**
 * Token拦截器的处理接口。
 * 
 * @author 王兵[wangchangbing@gmail.com]
 * 
 */
public interface RequestTokenHandler {

	/**
	 * 验证token是否合法，合法返回true，不合法返回相关的提示。
	 * 
	 */
	public boolean validateToken(HttpServletRequest request);
	
	/**
	 * 生成token
	 * 
	 * @param request
	 * @return
	 */
	public String generateToken(HttpServletRequest request);
}
