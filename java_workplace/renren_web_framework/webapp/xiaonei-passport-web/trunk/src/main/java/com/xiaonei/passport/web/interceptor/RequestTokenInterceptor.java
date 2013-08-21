package com.xiaonei.passport.web.interceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import com.xiaonei.commons.interceptors.access.annotation.ValidateRequestToken;
import com.xiaonei.commons.interceptors.access.anticsrf.DefaultRequestTokenHandler;
import com.xiaonei.commons.interceptors.access.anticsrf.RequestTokenHandler;
@Service
public class RequestTokenInterceptor extends HandlerInterceptorAdapter {
public static final String REQ_TOKEN_ATTRIBUTE_NAME = "_rtk";
	
	protected Log logger = LogFactory.getLog(this.getClass());
	private RequestTokenHandler handler = new DefaultRequestTokenHandler();
	
	@Override
	public boolean preHandle(HttpServletRequest request,
			HttpServletResponse response, Object handlerObject) throws Exception {

		String requestToken = (String) request
				.getAttribute(REQ_TOKEN_ATTRIBUTE_NAME);

		// 如果已经有了，就不重复设置了
		if (requestToken != null) {
			if (logger.isDebugEnabled()) {
				logger.debug(REQ_TOKEN_ATTRIBUTE_NAME
						+ " already exists: " + requestToken);
			}
		} else {
			String newToken = handler.generateToken(request);
			if (logger.isDebugEnabled()) {
				logger.debug("generated token: " + newToken);
			}
			request.setAttribute(REQ_TOKEN_ATTRIBUTE_NAME, newToken);
		}

		ValidateRequestToken token = handlerObject.getClass().getAnnotation(ValidateRequestToken.class);
		if (token == null || (token != null && !token.value())) {// 有token注释，且明确不验证。
			return true;
		}
		
		if (handler.validateToken(request)) {
			return true;
		} else {
			logger.error("error:405; Request not allowed.");
			return false;
		}
	}
	
}
