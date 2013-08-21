package com.xiaonei.commons.interceptors.access.anticsrf;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.interceptors.access.annotation.ValidateRequestToken;

/**
 * RequestTokenInterceptor的作用是防范CSRF攻击
 * 
 * @author 李伟博 (weibo.li@opi-corp.com)
 * @since 2010-4-19 下午07:16:16
 */
@Interceptor(oncePerRequest = true)
public class RequestTokenInterceptor extends ControllerInterceptorAdapter {

	public static final String REQ_TOKEN_ATTRIBUTE_NAME = "_rtk";
	
	protected Log logger = LogFactory.getLog(this.getClass());

	public RequestTokenInterceptor() {
		setPriority(29901); // 1 larger than PassportValidationInterceptor
	}
	
	private RequestTokenHandler handler = new DefaultRequestTokenHandler();

	@Override
	public Object before(Invocation inv) throws Exception {
		
		HttpServletRequest request = inv.getRequest();

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

		/**
		 * 安全中心验证 by david on 2012-05-17
		 */
		if(!request.getMethod().toLowerCase().equals("post"))return true;//不处理非POST请求
		ValidateRequestToken token = inv.getAnnotation(ValidateRequestToken.class);
		if (null!=token&&!token.value()) { // 有token注释，且明确不验证
			return true;
		} 
		
		if (handler.validateToken(request)) {
			return true;
		} else {
			return "error:405; Request not allowed.";
		}
	}
}
