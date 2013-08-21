package com.renren.sns.wiki.utils.concurrent;

import java.util.concurrent.Future;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author yi.li@renren-inc.com since 2011-12-2
 * 
 */
public class AsynTask implements Runnable {

	private Log logger = LogFactory.getLog(getClass());

	private static final long EXECUTE_WARNTIME = 1000;

	private AsynTaskExecutor executor;

	private Runnable task;

	private Throwable throwable;

	private long taskStartTime; // 任务开始时间

	private long timeToExe; // 任务执行允许的最大时间

	private String taskName; // 任务名

	private String taskNo; // 任务流水号

	private Future<?> future;

	public AsynTask(String taskName, String taskNo, Runnable task,
			AsynTaskExecutor executor, long timeToExe) {
		this.taskName = taskName;
		this.taskNo = taskNo;
		this.task = task;
		this.executor = executor;
		this.timeToExe = timeToExe;
	}

	public long getTimeToExe() {
		return timeToExe;
	}

	public void setTimeToExe(long timeToExe) {
		this.timeToExe = timeToExe;
	}

	public Future<?> getFuture() {
		return future;
	}

	public void setFuture(Future<?> future) {
		this.future = future;
	}

	public String getTaskName() {
		return taskName;
	}

	public String getTaskNo() {
		return taskNo;
	}

	public long getTaskStartTime() {
		return taskStartTime;
	}

	public Throwable getThrowable() {
		return throwable;
	}

	public void setThrowable(Throwable throwable) {
		this.throwable = throwable;
	}

	public AsynTaskExecutor getExecutor() {
		return executor;
	}

	@Override
	public void run() {
		try {
			taskStartTime = System.currentTimeMillis();
			executor.onTaskStart(this);
			task.run();
			long taskEndTime = System.currentTimeMillis();
			long exetime = taskEndTime - taskStartTime;
			if (exetime > EXECUTE_WARNTIME) {
				logger.warn(String.format(
						"AsynTask[%s] execute for a long time(%d)",
						this.toString(), exetime));
			}
			executor.onTaskEnd(this);
		} catch (Exception e) {
			this.throwable = e;
			executor.onTaskError(this);
		}
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}

		if (!(obj instanceof AsynTask)) {
			return false;
		}
		AsynTask _task = (AsynTask) obj;
		return (this.taskName == _task.taskName || (null != this.taskName && this.taskName
				.equals(_task.taskName)))
				&& (this.taskName == _task.taskNo || (null != this.taskNo && this.taskNo
						.equals(_task.taskNo)));
	}

	@Override
	public int hashCode() {
		int hashCode = 27;
		hashCode = 31 * hashCode
				+ (this.taskName == null ? 0 : this.taskName.hashCode());
		hashCode = 31 * hashCode
				+ (this.taskNo == null ? 0 : this.taskNo.hashCode());
		return hashCode;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("[").append(this.taskName).append(" - ");
		sb.append(this.taskNo).append("]");
		return sb.toString();
	}

	public static void main(String[] args) {
		AsynTask at1 = new AsynTask("testtask", "04", null, null, 0L);
		AsynTask at2 = new AsynTask("testtask", "04", null, null, 0L);
		System.out.println(at1.equals(at2) + " " + at2.equals(at1) + " + " + (at1.hashCode() == at2.hashCode()));
	}

}
