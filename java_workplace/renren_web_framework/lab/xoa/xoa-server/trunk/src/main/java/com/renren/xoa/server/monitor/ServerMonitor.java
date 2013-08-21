package com.renren.xoa.server.monitor;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.log4j.Logger;

/**
 * 服务器一些运行状态的监控
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-16 下午03:39:46
 */
public class ServerMonitor {

	private static ServerMonitor instance = new ServerMonitor();
	
	private static final int MONITOR_INTERVAL = 10;//seconds
	
	public static ServerMonitor getInstance() {
		return instance;	
	}
	
	private ScheduledExecutorService timer;
	
	private long startTime;
	
	private Logger logger = Logger.getLogger(ServerMonitor.class);
	
	private AtomicInteger requestReceived = new AtomicInteger();
	
	private AtomicInteger requestQueued = new AtomicInteger();
	
	private AtomicInteger responseSent = new AtomicInteger();
	
	private AtomicInteger workingThread = new AtomicInteger();
	
	private ServerMonitor() {
		try {
			startTimer();
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	private void startTimer() {
		
		if (logger.isInfoEnabled()) {
			logger.info("xoa-server-monitor started");
		}
		
		timer = Executors.newSingleThreadScheduledExecutor(new ThreadFactory() {
			@Override
			public Thread newThread(Runnable r) {
				return new Thread(r, "xoa-server-monitor-thread");
			}
		});
		
		timer.scheduleAtFixedRate(new Runnable() {
			
			@Override
			public void run() {
				try {
					monitor();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}, MONITOR_INTERVAL, MONITOR_INTERVAL, TimeUnit.SECONDS);
		
		startTime = System.currentTimeMillis();
	}
	
	/*public int incWorkingThread() {
		return workingThread.incrementAndGet();
	}
	
	public int descWorkingThread() {
		return workingThread.decrementAndGet();
	}*/
	
	public void setWorkingThread(AtomicInteger workingThread) {
		this.workingThread = workingThread;
	}
	
	public int incWorkingThread() {
		return workingThread.incrementAndGet();
	}
	
	public int descWorkingThread() {
		return workingThread.decrementAndGet();
	}
	
	public int incRequestReceived() {
		return requestReceived.incrementAndGet();
	}
	
	public int incResponseSent() {
		return responseSent.incrementAndGet();
	}
	
	public int incRequestQueued() {
		return requestQueued.incrementAndGet();
	}
	
	public int descRequestQueued() {
		return requestQueued.decrementAndGet();
	}
	
	private void monitor() {
		if (logger.isInfoEnabled()) {
			
			long curTime = System.currentTimeMillis();
			int duration = (int)((curTime - startTime) / 1000);
			
			int rr = requestReceived.get();
			int rq = requestQueued.get();
			int rs = responseSent.get();
			String s = "requestReceived=" + rr + ", " + 1.0 * rr / duration + "/s" + "\r\n"
				+ "requestQueued=" + rq + "\r\n"
				+ "workingThread=" + workingThread.get() + "\r\n"
				+ "responseSent=" + rs + ", " + 1.0 * rs / duration + "/s";
			logger.info(s);
		}
	}
	
}
