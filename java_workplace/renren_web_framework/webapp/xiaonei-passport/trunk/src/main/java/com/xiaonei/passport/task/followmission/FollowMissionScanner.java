package com.xiaonei.passport.task.followmission;

import java.util.ArrayList;
import java.util.Collection;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class FollowMissionScanner implements FollowMissionQueueDef {

	/**
	 * DateDefinition <br>
	 * 
	 * @author tai.wang@opi-corp.com Apr 7, 2010 - 9:36:35 AM
	 */
	public static class DD {

		public static int drainedMaxSize = 100;

		public static int nThreads = drainedMaxSize;

		public static int schedulePeriod = 10;

		public static ScheduledExecutorService schedule = Executors
				.newScheduledThreadPool(1);
		
		public static ExecutorService executor = Executors
				.newFixedThreadPool(DD.nThreads);

	}

	private class ScannerTimerTask implements Runnable {

		@Override
		public void run() {
			while (!queue.isEmpty()) {
				FollowMissionScanner.this.scanTheQueueOnce();
			}
		}

	}

	private final static FollowMissionScanner instance = new FollowMissionScanner();

	static {
		scanTheQueue(DD.schedule);
	}

	public static FollowMissionScanner getInstance() {
		return instance;
	}

	/**
	 * scanTheQueue<br>
	 * 
	 * 
	 * @author tai.wang@opi-corp.com Apr 6, 2010 - 11:03:37 AM
	 */
	public static void scanTheQueue(ScheduledExecutorService schedule) {
		schedule.scheduleAtFixedRate(getInstance().new ScannerTimerTask(), 0,
				DD.schedulePeriod, TimeUnit.MILLISECONDS);
	}

	public void insertInQueue(Collection<IFollowMissionTask> tasks) {
		if (tasks == null) {
			return;
		}
		try {
			queue.addAll(tasks);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * insertInQueue<br>
	 * 
	 * @param task
	 * 
	 * @author tai.wang@opi-corp.com Apr 6, 2010 - 11:03:39 AM
	 */
	public void insertInQueue(IFollowMissionTask task) {
		if (task == null) {
			return;
		}
		try {
			queue.offer(task);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * scanTheQueueOnce<br>
	 * 
	 * 
	 * @author tai.wang@opi-corp.com Apr 6, 2010 - 11:03:34 AM
	 */
	private void scanTheQueueOnce() {
		Collection<IFollowMissionTask> c = new ArrayList<IFollowMissionTask>();
		queue.drainTo(c, DD.drainedMaxSize);
		for (IFollowMissionTask task : c) {
			// new Thread(task).start();
			// task.run(); // sync
			DD.executor.execute(task);
		}
	}
}
