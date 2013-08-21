package com.xiaonei.reg.register.logic.additional;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;

public class Singleton {

	private static Map<String, Object> mRegistry = new HashMap<String, Object>();

	@SuppressWarnings("unchecked")
	public static <T> T getInstance(Class<T> clazz) {
		if (clazz == null) {
			return null;
		}
		if (mRegistry.get(clazz.getName()) == null) {
			try {
				Constructor<?> cxtor = Class.forName(clazz.getName())
						.getDeclaredConstructor();
				cxtor.setAccessible(true);
				mRegistry.put(clazz.getName(), cxtor.newInstance());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return (T) (mRegistry.get(clazz.getName()));
	}
}
