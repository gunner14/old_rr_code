package com.xiaonei.reg.common;

import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class RunnableTaskExecutor {
	private RunnableTaskExecutor() {
	}

	private static final RunnableTaskExecutor rte = new RunnableTaskExecutor();

	public static RunnableTaskExecutor getInstance() {
		return rte;
	}

	private static final Executor executor = Executors.newCachedThreadPool();
	private static final Executor executorForUserTrace = Executors.newFixedThreadPool(50);

	public void runTaskForUserTrace(final Runnable task) {
		executorForUserTrace.execute(task);
	}
	
	public void runTask(final Runnable task) {
		executor.execute(task);
	}

	public void runConcurrentTasks(final String groupName,
			final Map<String, Thread> tasksMap) {
		runConcurrentTasks(groupName, tasksMap, null, null);
	}

	@Deprecated
	public void runConcurrentTasks(final String groupName,
			final Map<String, Thread> tasksMap,
			final ObjectHolder<?> oh, final Map<String, Object> returns) {
		final RegLogger logger = RegLogger.getLoggerWithType("concurrent task");
		for (final Entry<String, Thread> task : tasksMap.entrySet()) {
			task.getValue().start();
			logger.info("group name:" + groupName + " task name:"
					+ task.getKey() + " -- start");
		}
		for (final Entry<String, Thread> task : tasksMap.entrySet()) {
			try {
				task.getValue().join();
				logger.info("group name:" + groupName + " task name:"
						+ task.getKey() + " -- over");
			} catch (final InterruptedException e) {
				e.printStackTrace();
				logger.info("group name: " + groupName + "task name: "
						+ task.getKey() + " -- interrupted");
			}
		}
	}
}