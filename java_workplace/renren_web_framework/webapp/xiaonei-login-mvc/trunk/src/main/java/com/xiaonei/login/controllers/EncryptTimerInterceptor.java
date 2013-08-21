package com.xiaonei.login.controllers;

import java.lang.annotation.Annotation;
import java.util.Map;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.LoginConfigureImpl;
import com.xiaonei.login.logic.logger.PpLoggerLocalMsg;
import com.xiaonei.login.logic.validator.annotation.EncryptTimer;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

public class EncryptTimerInterceptor extends ControllerInterceptorAdapter {
	
	private ThreadLocal<Long> timeCost = new ThreadLocal<Long>();

	public EncryptTimerInterceptor() {
		// 设置为最高级别
		setPriority(99999);
	}
	
	@Override
	protected Class<? extends Annotation> getRequiredAnnotationClass() {
        return EncryptTimer.class;
    }
	
	@Override
	public Object before(Invocation inv) throws Exception {
		timeCost.set(System.currentTimeMillis());
		PpLoggerLocalMsg.initial();
		return super.before(inv);
	}

	@Override
	public Object after(Invocation inv, Object instruction) throws Exception {
		Map<String, String> config = LoginConfigureImpl.getInstance().getLoginConfigure();
		String isTimeEncrypt = config.get("isTimeEncrypt");
		if (isTimeEncrypt != null && "true".equalsIgnoreCase(isTimeEncrypt)) {
			timeCost.set(System.currentTimeMillis() - timeCost.get());
			String controllerName = inv.getControllerClass().getSimpleName();
			LoginLoggerDef.logEncrypt.info(controllerName + " cost "
					+ timeCost.get());
		}
		timeCost.remove();
		return super.after(inv, instruction);
	}
}
