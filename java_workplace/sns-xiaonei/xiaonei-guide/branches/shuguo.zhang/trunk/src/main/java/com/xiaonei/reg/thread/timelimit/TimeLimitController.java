package com.xiaonei.reg.thread.timelimit;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import com.xiaonei.reg.thread.timelimit.TimeLimitConfiguration;

public class TimeLimitController {

	/**
	 * 获得TimeLimitConfiguration对象
	 * 
	 * @return
	 */
	public static TimeLimitConfiguration getTimeLimit(int maxRetryTimes,
			long onceWaitTime) {
		try {
			Constructor<TimeLimitConfiguration> cxtor = TimeLimitConfiguration.class
					.getDeclaredConstructor(Integer.class, Long.class);
			cxtor.setAccessible(true);
			return cxtor.newInstance(maxRetryTimes, onceWaitTime);
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * 获得TimeLimitConfiguration对象
	 * 
	 * @return
	 */
	public static TimeLimitConfiguration getTimeLimit(long waitTime) {
		return getTimeLimit(1, waitTime);
	}

	/**
	 * 获得TimeLimitConfiguration对象
	 * 
	 * @return
	 */
	public static TimeLimitConfiguration getTimeLimit(int retryTimes) {
		return getTimeLimit(retryTimes, 1);
	}

}