/**
 * 
 */
package com.xiaonei.reg.common.proxy;

import java.lang.reflect.Constructor;

import net.sf.cglib.proxy.Callback;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;


/**
 * @author wangtai
 * 
 */
public abstract class ProxyTemplate implements MethodInterceptor {
	private Enhancer enhancer = new Enhancer();

	// 返回子类
	@SuppressWarnings("unchecked")
	public <T> T get(Class<T> clz) {
		Constructor<?> cxtor;
		try {
			cxtor = Class.forName(clz.getName())
			.getDeclaredConstructor();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		try {
			enhancer.setSuperclass(clz);
			enhancer.setCallback((Callback) this);
			return (T) enhancer.create();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
			try {
				cxtor.setAccessible(true);
				return (T)cxtor.newInstance();
			} catch (Exception e1) {
				throw new RuntimeException(e1);
			}
		}
	}
}
