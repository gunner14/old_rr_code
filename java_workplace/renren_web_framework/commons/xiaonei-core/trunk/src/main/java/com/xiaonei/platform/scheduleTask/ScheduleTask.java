package com.xiaonei.platform.scheduleTask;

import static java.util.concurrent.TimeUnit.SECONDS;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

public class ScheduleTask {

	private Domain domain;

	public static ScheduleTask initialize(DatabaseHelper ds) {
		try {
			return new ScheduleTask(ds);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	private ScheduleTask(DatabaseHelper ds) throws Exception {
		try {			
			domain = new Domain(ds);
			new MonitorTask(domain);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String args[]) {
		int delaytime = 60;
		if (args.length > 0) {
			delaytime = Integer.parseInt(args[0]);
		}
		final ScheduledExecutorService scheduler = Executors
				.newScheduledThreadPool(1);
		final Runnable task = new Runnable() {
			public void run() {				
				ScheduleTask.initialize(new DatabaseHelperImpl());
			}
		};
		scheduler.scheduleWithFixedDelay(task, 0, delaytime, SECONDS);
	}
}
