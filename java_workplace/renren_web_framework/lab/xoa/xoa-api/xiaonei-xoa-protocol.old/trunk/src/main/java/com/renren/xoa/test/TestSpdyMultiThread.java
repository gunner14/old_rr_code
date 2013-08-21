package com.renren.xoa.test;

import java.net.URISyntaxException;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.client.service.CommunicationClient;
import com.renren.xoa.client.service.CommunicationService;
import com.renren.xoa.client.service.ResponseObserver;
import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-16 下午05:20:09
 */
public class TestSpdyMultiThread {

	private AtomicBoolean running = new AtomicBoolean(true);
	
	private AtomicInteger sent = new AtomicInteger();
	
	private AtomicInteger succ = new AtomicInteger();
	
	private AtomicInteger fail = new AtomicInteger();
	
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
			System.out.println(duration + "s (sent:" + s + ", " + 1.0 * s / duration
					+ "/s), (received:" + r + ", " + 1.0 * r / duration + "/s), (fail:" + f + ")");
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
		
		private CommunicationClient client;
		
		public TestTask(String domain, String path) {
			this.domain = domain;
			this.path = path;
			 try {
				client = CommunicationService.getInstance()
					.getClient("xoa://" + domain);
			} catch (URISyntaxException e) {
				e.printStackTrace();
			}
		}
		
		@Override
		public void run() {
			int count = 0;
			while (true) {
				if (count < 10000) {
					try {
						SpdyHttpRequest shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1,
								HttpMethod.GET, path);
						client.send(shr, new ResponseObserver() {
							@Override
							public void messageReceived(SpdyHttpResponse response) {
								int status = response.getStatus().getCode();
								if (status == 200) {
									succ.incrementAndGet();
								} else {
									System.err.println("bad status:" + status);
									fail.incrementAndGet();
								}
							}
						});
						sent.incrementAndGet();
						//Thread.sleep(100);
					} catch (Exception e) {
						e.printStackTrace();
						fail.incrementAndGet();
					}
				} else {
					try {
						Thread.sleep(300);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					count = 0;
				}
				count ++;
			}
		}
	}
	
	private static void multiThreadTest(String domain, String path, int nThreads) {
		
		System.out.println("start spdy multi-thread test");
		/*String domain = "10.3.21.36:8888";
		String path = "/spdy/index.jsp";*/
		System.out.println(domain + path);
		TestSpdyMultiThread app = new TestSpdyMultiThread();
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
