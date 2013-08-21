package com.xiaonei.platform.core.schedule;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:38:48
 */
public class ScheduleService {

	private static ScheduledExecutorService timer = Executors
			.newSingleThreadScheduledExecutor();

	public static ScheduledExecutorService getService() {
		return timer;
	}
	
}
