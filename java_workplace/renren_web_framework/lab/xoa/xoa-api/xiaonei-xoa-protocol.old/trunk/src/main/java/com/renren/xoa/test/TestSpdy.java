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
 * @since 2010-3-9 下午04:31:47
 */
public class TestSpdy {

	private AtomicBoolean running = new AtomicBoolean(true);
	
	private AtomicInteger sent = new AtomicInteger();
	
	private AtomicInteger succ = new AtomicInteger();
	
	private AtomicInteger fail = new AtomicInteger();
	
	private ScheduledExecutorService threadPool = Executors.newScheduledThreadPool(100);
	
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
			while (running.get()) {
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
					Thread.sleep(10);
					
				} catch (Exception e) {
					e.printStackTrace();
					fail.incrementAndGet();
				}
			}
		}
	}
	
	private static void singleThreadTest(String domain, String path) {
		
		System.out.println("start single thread test");
		/*String domain = "10.3.21.36:8888";
		String path = "/spdy/index.jsp";*/
		System.out.println(domain + path);
		TestSpdy app = new TestSpdy();
		app.startMonitor();
		app.threadPool.execute(app.new TestTask(domain, path));
	}
	
	public static void main(String[] args) {
		
		if (args.length != 2) {
			System.out.println("Usage:java xxx domain path");
			return;
		} 
		
		singleThreadTest(args[0], args[1]);
	}
	
}
