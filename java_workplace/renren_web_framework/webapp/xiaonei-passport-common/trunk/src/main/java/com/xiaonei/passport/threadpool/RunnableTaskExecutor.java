package com.xiaonei.passport.threadpool;

import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.slf4j.Logger;

/**
 * RunnableTaskExecutor
 * 
 * @author tai.wang@opi-corp.com Dec 7, 2009 - 3:56:17 PM
 */
public class RunnableTaskExecutor {
	 private final static Log logger = LogFactory.getLog(RunnableTaskExecutor.class);
		private static final ExecutorService executor = Executors
				.newCachedThreadPool();

		private static final ExecutorService executorForLogger = new ThreadPoolExecutor(
				100, 100, 0L, TimeUnit.MILLISECONDS,
				new LinkedBlockingQueue<Runnable>(5000));
		private static final ExecutorService executorForFM = new ThreadPoolExecutor(
				500, 800, 2L, TimeUnit.MILLISECONDS,
				new LinkedBlockingQueue<Runnable>(7000));

		private static final RunnableTaskExecutor instance = new RunnableTaskExecutor();

		public static RunnableTaskExecutor getInstance() {
			return instance;
		}

		private RunnableTaskExecutor() {
		}

		public void runConcurrentTasks(final String groupName,
				final Map<String, Thread> tasksMap, Logger logger) {
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

		public void runTask(final Runnable task) {
			executor.execute(task);
		}

		public void runTaskForLogger(final Runnable task) {
			executorForLogger.execute(task);
		}

		public void runTaskForFM(final Runnable task) {
			try{
				executorForFM.execute(task);
			}catch(Exception e){
				logger.error("record log error", e);
			}
			
		}
	}
