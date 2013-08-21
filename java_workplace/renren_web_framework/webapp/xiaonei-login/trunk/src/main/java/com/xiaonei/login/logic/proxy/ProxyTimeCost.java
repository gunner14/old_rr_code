package com.xiaonei.login.logic.proxy;

import java.lang.reflect.Method;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.login.constants.LoginLoggerDef;

import net.sf.cglib.proxy.MethodProxy;

public class ProxyTimeCost extends ProxyTemplate {

	private ProxyTimeCost() {
	}

	public static <T> T getLogProxy(Class<T> clazz) {
		return (T) new ProxyTimeCost().get(clazz);
	}

	private Long timeCost;

	private String methodName = "";

	@Override
	protected void start(Object o, Method method, Object[] args,
			MethodProxy proxy) {
		String[] classNameA = StringUtils.split(o.getClass().getName(), "\\.");
		String className = classNameA[classNameA.length - 1].split("\\$\\$")[0];
		String sMethodName = method.getName();
		this.methodName = className + ":" + sMethodName;
		this.timeCost = System.currentTimeMillis();
		// System.out.println(methodName.get() + " start");
	}

	@Override
	protected void over(Object o, Method method, Object[] args,
			MethodProxy proxy, Object result) {
		long lTimeCost = System.currentTimeMillis() - timeCost;
		LoginLoggerDef.logTimeCost.info(methodName + " cost " + lTimeCost);
	}

}
