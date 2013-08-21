package com.xiaonei.commons.interceptors.access.anticsrf;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.utility.anticsrf.AntiCsrfUtil;

/**
 * {@link RequestTokenInterceptor}拦截器处理类，{@link RequestTokenHandler}的实现类。<br/>
 * 
 * @author 王兵[wangchangbing@gmail.com]
 * 
 */
public class DefaultRequestTokenHandler implements RequestTokenHandler {

	/**
	 * 取出requestToken的值，检查token是否合法。
	 * 
	 * @param requestToken
	 * @param inv
	 */
	public boolean validateToken(HttpServletRequest request) {
		return AntiCsrfUtil.validateToken(request, RequestTokenInterceptor.REQ_TOKEN_ATTRIBUTE_NAME);
	}

	@Override
	public String generateToken(HttpServletRequest request) {
		return AntiCsrfUtil.generateToken(request);
	}
}
