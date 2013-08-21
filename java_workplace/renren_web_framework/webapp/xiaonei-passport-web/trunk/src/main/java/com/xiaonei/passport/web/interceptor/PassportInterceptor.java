package com.xiaonei.passport.web.interceptor;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Service;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import com.xiaonei.passport.logger.LoginLoggerDef;
import com.xiaonei.passport.logger.PpLoggerLocalMsg;
import com.xiaonei.passport.web.annotation.TimeCost;

@Service
public class PassportInterceptor extends HandlerInterceptorAdapter{
	private ThreadLocal<Long> timeCost = new ThreadLocal<Long>();

	@Override
	public void afterCompletion(HttpServletRequest request,
			HttpServletResponse response, Object handler, Exception ex)
			throws Exception {
		timeCost.set(System.currentTimeMillis() - timeCost.get());
		String methodName = handler.getClass().getSimpleName();
		LoginLoggerDef.logTimeCost.info(methodName + " cost " + timeCost.get());
		timeCost.remove();
		super.afterCompletion(request, response, handler, ex);
	}


	@Override
	public boolean preHandle(HttpServletRequest request,
			HttpServletResponse response, Object handler) throws Exception {
		if (!handler.getClass().isAnnotationPresent(TimeCost.class)) {
			return true;
		}
		timeCost.set(System.currentTimeMillis());
		PpLoggerLocalMsg.initial();
		return super.preHandle(request, response, handler);
	}
}
