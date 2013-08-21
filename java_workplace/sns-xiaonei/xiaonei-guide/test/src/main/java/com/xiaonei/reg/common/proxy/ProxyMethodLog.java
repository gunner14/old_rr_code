/**
 * 
 */
package com.xiaonei.reg.common.proxy;

import java.lang.reflect.Method;

import net.sf.cglib.proxy.MethodProxy;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;

/**
 * @author wangtai
 * 
 */
public class ProxyMethodLog extends ProxyTemplate {

	private static final RegLogger logger = RegLogger.getLogger("regTimeCost",
			"RegLog");

	private ProxyMethodLog() {

	}

	private static ProxyMethodLog getNewInstance() {
		return new ProxyMethodLog();
	}

	public static <T> T getLogProxy(Class<T> clazz) {
		return (T) ProxyMethodLog.getNewInstance().get(clazz);
	}

	@Override
	public Object intercept(Object o, Method method, Object[] args,
			MethodProxy proxy) throws Throwable {

		String[] classNameA = StringUtils.split(o.getClass().getName(), "\\.");
		String className = classNameA[classNameA.length - 1].split("\\$\\$")[0];
		String methodName = method.getName();
		logger.info(className + ":" + methodName + ":start");
		long timeCost = System.currentTimeMillis();
		Object result = proxy.invokeSuper(o, args);
		logger.info(className + ":" + methodName + ":end:"
				+ (System.currentTimeMillis() - timeCost));
		return result;
	}
}
