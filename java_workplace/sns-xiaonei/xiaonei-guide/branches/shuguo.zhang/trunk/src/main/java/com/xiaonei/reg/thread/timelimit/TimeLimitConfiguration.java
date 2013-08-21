package com.xiaonei.reg.thread.timelimit;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.thread.timelimit.exception.TimeLimitNoReturnException;
import com.xiaonei.reg.thread.timelimit.exception.TimeLimitTimeoutException;

/**
 * 
 * @author wangtai
 * 
 */
public class TimeLimitConfiguration {

	private String rKey;
	private static final RegLogger logger = RegLogger
			.getLoggerWithType(TimeLimitConfiguration.class);
	private int maxRetryTimes = Integer.MAX_VALUE;
	private long onceWaitTime = 1;
	private Map<String, InnerTLThread> target = new HashMap<String, InnerTLThread>();
	private Map<String, Object> oReturn = new HashMap<String, Object>();

	/**
	 * task
	 * 
	 * @author wangtai
	 * 
	 */
	abstract class InnerTLThread extends Thread {

		private String retKey;
		private long timeCost;

		public InnerTLThread() {
			setDaemon(true);
		}

		public void setRetKey(String retKey) {
			this.retKey = retKey;
		}

		@Override
		public void run() {
			timeCost = System.currentTimeMillis();
			logger.debug(retKey + " started");
			oReturn.put(retKey, work());
		}

		public void shutdown() {
			Object oValue = oReturn.get(retKey);
			String sValue = "";
			if (oValue instanceof String) {
				sValue = "value:" + oValue;
			}
			if (oValue instanceof Integer) {
				sValue = "value:" + oValue;
			}
			if (oValue instanceof Long) {
				sValue = "value:" + oValue;
			}
			if (oValue instanceof Double) {
				sValue = "value:" + oValue;
			}
			if (oValue instanceof Boolean) {
				sValue = "value:" + oValue;
			}
			if (oValue instanceof Byte) {
				sValue = "value:" + oValue;
			}
			logger.debug(retKey + " over, " + sValue + " timeCost:"
					+ (System.currentTimeMillis() - timeCost));
		}

		public void timeoutShutdown() {
			if (super.isAlive()) {
				super.interrupt();
			}
			logger.debug(retKey + " timeout interrput");
		}

		public abstract Object work();
	}

	private TimeLimitConfiguration(Integer maxRetryTimes, Long onceWaitTime) {
		this.maxRetryTimes = maxRetryTimes;
		if(Globals.DEBUG_ENV){
			this.maxRetryTimes = Integer.MAX_VALUE;
		}
		this.onceWaitTime = onceWaitTime;
	}

	/**
	 * 设置单独的TimeLimitThread
	 * 
	 * @param target
	 */
	public void setSingleConfig(String id, TimeLimitThread target) {
		this.target = new HashMap<String, InnerTLThread>();
		this.oReturn = new HashMap<String, Object>();
		rKey = id;
		this.target.put(id, target);
	}

	/**
	 * 获得返回值
	 * 
	 * @return
	 * @throws TimeLimitTimeoutException
	 * @throws TimeLimitNoReturnException
	 */
	public Object getSingleReturn() throws TimeLimitTimeoutException,
			TimeLimitNoReturnException {
		String key = rKey;
		this.taskManageSingleTarget(key);
		return oReturn.get(key);
	}

	/**
	 * 设置一组 Thread
	 * 
	 * @param m
	 */
	public void setMulitpleConfig(Map<String, TimeLimitThread> m) {
		this.target = new HashMap<String, InnerTLThread>();
		this.oReturn = new HashMap<String, Object>();
		for (Entry<String, TimeLimitThread> t : m.entrySet()) {
			this.target.put(t.getKey(), t.getValue());
		}
	}

	/**
	 * 获得所有返回值
	 * 
	 * @return
	 */
	public Map<String, Object> getMultipleReturn()
			throws TimeLimitNoReturnException {
		this.taskManageMultipleTarget();
		return oReturn;
	}

	private void taskManageMultipleTarget() {
		List<Thread> tlist = new ArrayList<Thread>();
		for (String key : target.keySet()) {
			runSingleTargetInList(key, tlist);
		}
		for (Thread to : tlist) {
			try {
				joinSingleTargetInList(to);
			} catch (TimeLimitTimeoutException e) {
				e.printStackTrace();
			}
		}
	}

	private void joinSingleTargetInList(Thread to)
			throws TimeLimitTimeoutException {
		try {
			to.join();
		} catch (InterruptedException e) {
			throw new TimeLimitTimeoutException(e);
		}
	}

	private void runSingleTargetInList(String key, List<Thread> tlist) {
		final String fkey = key;
		final Thread to = new Thread() {
			@Override
			public void run() {
				try {
					taskManageSingleTarget(fkey);
				} catch (Exception e) {
					throw new RuntimeException(e);
				}
			}
		};
		to.start();
		tlist.add(to);
	}

	private void taskManageSingleTarget(String key)
			throws TimeLimitTimeoutException {
		InnerTLThread task = target.get(key);
		task.setRetKey(key);
		RunnableTaskExecutor.getInstance().runTask(task);
		for (int i = 1; i <= maxRetryTimes; i++) {
			onceWaiting(task);
			if (null != oReturn.get(key)) {
				stopThread(key);
				break;
			}
			if ((i) == maxRetryTimes) {
				timeoutStopThread(key);
				throw new TimeLimitTimeoutException("Timeout");
			}
		}
	}

	private void stopThread(String key) {
		target.get(key).shutdown();
	}

	private void timeoutStopThread(String key) {
		target.get(key).timeoutShutdown();
	}

	private void onceWaiting(Thread task) throws TimeLimitTimeoutException {
		try {
			Thread.sleep(onceWaitTime);
		} catch (InterruptedException e) {
			throw new TimeLimitTimeoutException(e);
		}
		// task.join(onceWaitTime);
	}

}