package com.renren.xoa.test;

import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaMethod;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.method.XoaGetMethod;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-16 下午05:20:09
 */
public class TestXoaClientMultiThread {

	private AtomicBoolean running = new AtomicBoolean(true);
	
	private AtomicInteger sent = new AtomicInteger();
	
	private AtomicInteger succ = new AtomicInteger();
	
	private AtomicInteger fail = new AtomicInteger();
	
	private AtomicLong totalTime = new AtomicLong();
	
	private volatile int minTime = 1000;
	
	private volatile int maxTime = 0;
	
	private ScheduledExecutorService threadPool = Executors.newScheduledThreadPool(250);
	
	private long startTime;
	
	private Runnable monitor = new Runnable() {
		@Override
		public void run() {
			long currentTime = System.currentTimeMillis();
			int duration = (int)((currentTime - startTime) / 1000);
			int s = sent.get();
			int r = succ.get();
			int f = fail.get();
			long t = totalTime.get();
			System.out.println(duration + "s (sent:" + s + ", " + 1.0 * s / duration
					+ "/s), (received:" + r + ", " + 1.0 * r / duration + "/s), (avgTime:" + t / r + "ms, " +
					"maxTime:" + maxTime + "ms, minTime:" + minTime + "), " +	
					"(fail:" + f + ")");
		}
	};
	
	private void startMonitor() {
		startTime = System.currentTimeMillis();
		int duration = 10;
		threadPool.scheduleAtFixedRate(monitor, duration, duration, TimeUnit.SECONDS);
	}
	
	private class TestTask implements Runnable {

		private String domain;
		
		private String path;
		
		private XoaClient client = new XoaClient();
		
		public TestTask(String domain, String path) {
			this.domain = domain;
			this.path = path;
		}
		
		@Override
		public void run() {
			while (running.get()) {
				try {
					XoaMethod method = new XoaGetMethod(domain, path);
					long startTime = System.currentTimeMillis();
					Future<XoaResponse> f = client.submitMethod(method);
					sent.incrementAndGet();
					XoaResponse res = f.get();
					long endTime = System.currentTimeMillis();
					int time = (int) (endTime - startTime);
					if (time > maxTime) {
						maxTime = time;
					}
					if (time < minTime) {
						minTime = time;
					}
					
					totalTime.addAndGet(time);
					succ.incrementAndGet();
					//Thread.sleep(1000);
				} catch (Exception e) {
					e.printStackTrace();
					fail.incrementAndGet();
				}
			}
		}
	}
	
	private static void multiThreadTest(String domain, String path, int nThreads) {
		
		System.out.println("start xoa-client multi-thread test");
		/*String domain = "10.3.21.36:8888";
		String path = "/spdy/index.jsp";*/
		System.out.println(domain + path);
		TestXoaClientMultiThread app = new TestXoaClientMultiThread();
		app.startMonitor();
		for (int i = 0; i < nThreads; i++) {
			app.threadPool.execute(app.new TestTask(domain, path));
		}
	}
	
	public static void main(String[] args) {
		
		if (args.length != 3) {
			System.out.println("Usage:java xxx domain path concurrency");
			return;
		} 
		
		int con = Integer.parseInt(args[2]);
		
		if (con > 200) {
			System.out.println("concurrency too big:" + con);
			return;
		}
		
		multiThreadTest(args[0], args[1], con);
	}
}
