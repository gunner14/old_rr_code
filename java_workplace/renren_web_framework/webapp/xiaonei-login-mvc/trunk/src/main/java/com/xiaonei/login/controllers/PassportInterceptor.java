package com.xiaonei.login.controllers;

import java.lang.reflect.Method;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.annotation.TimeCost;
import com.xiaonei.login.logic.logger.PpLoggerLocalMsg;

public class PassportInterceptor extends ControllerInterceptorAdapter {

	private ThreadLocal<Long> timeCost = new ThreadLocal<Long>();

	@Override
	protected boolean isForAction(Method actionMethod, Class<?> controllerClazz) {
		boolean defRt = super.isForAction(actionMethod, controllerClazz);
		if (null == controllerClazz) {
			return defRt;
		}
		if (controllerClazz.isAnnotationPresent(TimeCost.class)) {
			return true;
		}
		return defRt;
	}

	@Override
	public Object before(Invocation inv) throws Exception {
		timeCost.set(System.currentTimeMillis());
		PpLoggerLocalMsg.initial();
		return super.before(inv);
	}

	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		timeCost.set(System.currentTimeMillis() - timeCost.get());
		String methodName = inv.getControllerClass().getSimpleName();
		LoginLoggerDef.logTimeCost.info(methodName + " cost " + timeCost.get());
		timeCost.remove();
		return super.after(inv, instruction);
	}
}
