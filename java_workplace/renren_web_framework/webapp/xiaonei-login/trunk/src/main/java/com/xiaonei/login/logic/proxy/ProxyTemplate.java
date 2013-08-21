/**
 * 
 */
package com.xiaonei.login.logic.proxy;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

import net.sf.cglib.proxy.Callback;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

/**
 * ProxyTemplate <br>
 * 代理方法的模板,
 * 
 * @author tai.wang@opi-corp.com Jan 6, 2010 - 2:44:08 PM
 */
public abstract class ProxyTemplate implements MethodInterceptor {

	private Enhancer enhancer = new Enhancer();

	/**
	 * get
	 * 
	 * @param <T>
	 * @param clz
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Jan 8, 2010 - 12:34:55 PM
	 */
	public <T> T get(Class<T> clz) {
		Constructor<?> cxtor;
		try {
			cxtor = Class.forName(clz.getName()).getDeclaredConstructor();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		try {
			enhancer.setSuperclass(clz);
			enhancer.setCallback((Callback) this);
			return clz.cast(enhancer.create());
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
			try {
				cxtor.setAccessible(true);
				return clz.cast(cxtor.newInstance());
			} catch (Exception e1) {
				throw new RuntimeException(e1);
			}
		}
	}

	@Override
	public Object intercept(Object o, Method method, Object[] args,
			MethodProxy proxy) throws Throwable {
		start(o, method, args, proxy);
		Object result = proxy.invokeSuper(o, args);
		over(o, method, args, proxy, result);
		return result;
	}

	/**
	 * start<br>
	 * 在要代理的方法前执行
	 * 
	 * @param o
	 * @param method
	 * @param args
	 * @param proxy
	 * 
	 * @author tai.wang@opi-corp.com Jan 6, 2010 - 2:53:22 PM
	 */
	protected abstract void start(Object o, Method method, Object[] args,
			MethodProxy proxy);

	/**
	 * over<br>
	 * 在要代理的方法后执行
	 * 
	 * @param o
	 * @param method
	 * @param args
	 * @param proxy
	 * @param result
	 * 
	 * @author tai.wang@opi-corp.com Jan 6, 2010 - 2:54:00 PM
	 */
	protected abstract void over(Object o, Method method, Object[] args,
			MethodProxy proxy, Object result);
}
