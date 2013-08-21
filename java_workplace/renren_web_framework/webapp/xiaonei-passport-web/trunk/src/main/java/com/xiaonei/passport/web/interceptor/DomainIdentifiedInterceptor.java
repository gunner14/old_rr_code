package com.xiaonei.passport.web.interceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import com.xiaonei.platform.core.opt.OpiConstants;
@Service
public class DomainIdentifiedInterceptor extends HandlerInterceptorAdapter {

	@Override
	public void afterCompletion(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex)
			throws Exception {
		OpiConstants.clearCurrentDomain();
	}

	@Override
	public boolean preHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler) throws Exception {
		String domain = OpiConstants.getCurrentDomain();
		if (domain == null) {
			domain = OpiConstants.findDomain(request);
			OpiConstants.setCurrentDomain(domain);
			request.setAttribute("domainIdentified", OpiConstants
					.getCurrentDomainIdentifiedProperties());
		}
		return true;
	}

}
