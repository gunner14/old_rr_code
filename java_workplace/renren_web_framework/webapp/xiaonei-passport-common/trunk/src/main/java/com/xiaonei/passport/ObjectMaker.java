package com.xiaonei.passport;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;



public class ObjectMaker {

	private static final Map<String, Object> mRegistry = new HashMap<String, Object>();

	/**
	 * getInstance
	 * 
	 * @param <T>
	 * @param clazz
	 * @return
	 * 
	 * @author tai.wang@opi-corp.com Dec 25, 2009 - 4:23:14 PM
	 */
	@SuppressWarnings("unchecked")
	public static <T> T getInstance(Class<T> clazz) {
		if (clazz == null) {
			return null;
		}
		if (clazz.isInterface()) {
			return null;
		}
		if (clazz.isAnnotation()) {
			return null;
		}
		if (mRegistry.get(clazz.getName()) == null) {
			try {
				Constructor<?> cxtor = Class.forName(clazz.getName())
						.getDeclaredConstructor();
				cxtor.setAccessible(true);
				T t = null;
//				if (clazz.isAnnotationPresent(TimeCost.class)) {
//					t = ProxyTimeCost.getLogProxy(clazz);
//				} else {
					t = (T) cxtor.newInstance();
//				}
				mRegistry.put(clazz.getName(), t);
				return t;
			} catch (Exception e) {
				e.printStackTrace();
				return null;
			}
		}
		return (T) (mRegistry.get(clazz.getName()));
	}

	/**
	 * registerSingleton<br>
	 * 在指定singleton 的class 中的static 块中注册
	 * 
	 * @param clazz
	 * 
	 * @author tai.wang@opi-corp.com Dec 25, 2009 - 4:20:47 PM
	 */
	public static void registerSingleton(Class<?> clazz) {
		if (null == clazz) {
			return;
		}
		if (clazz.isInterface()) {
			return;
		}
		if (clazz.isAnnotation()) {
			return;
		}
//		if (!clazz.isAnnotationPresent(Singleton.class)) {
//			return;
//		}

		if (mRegistry.get(clazz.getName()) == null) {
			try {
				Constructor<?> cxtor = Class.forName(clazz.getName())
						.getDeclaredConstructor();
				cxtor.setAccessible(true);
//				if (clazz.isAnnotationPresent(TimeCost.class)) {
//					mRegistry.put(clazz.getName(), ProxyTimeCost
//							.getLogProxy(clazz));
//				} else {
					mRegistry.put(clazz.getName(), cxtor.newInstance());
//				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
