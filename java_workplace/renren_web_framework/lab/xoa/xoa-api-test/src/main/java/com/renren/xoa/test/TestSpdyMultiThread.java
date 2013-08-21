package com.renren.xoa.test;

import java.net.InetSocketAddress;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.client.service.ResponseObserver;
import com.renren.xoa.client.service.SpdyClient;
import com.renren.xoa.client.spdy.PowerfulSpdyClient;
import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-16 下午05:20:09
 */
public class TestSpdyMultiThread {
	
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
		
		private String path;
		
		private SpdyClient client;
		
		public TestTask(SpdyClient client, String path) {
			this.path = path;
			this.client = client;
		}
		
		@Override
		public void run() {
			while (true) {
				try {
					SpdyHttpRequest shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1,
							HttpMethod.GET, path);
					shr.setHeader("Host", "blog.xoa.renren.com");
					final CountDownLatch latch = new CountDownLatch(1);
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
							latch.countDown();
						}
					});
					sent.incrementAndGet();
					latch.await();
					Thread.sleep(1000);
					break;
				} catch (Exception e) {
					e.printStackTrace();
					fail.incrementAndGet();
				}
			}
		}
	}
	
	private static void multiThreadTest(String host, String path, int nThreads) {
		
		System.out.println("start spdy multi-thread test");
		/*String domain = "10.3.21.36:8888";
		String path = "/spdy/index.jsp";*/
		System.out.println(host + path);
		
		String[] ss = host.split(":");
		SpdyClient client = new PowerfulSpdyClient(new InetSocketAddress(ss[0], Integer.parseInt(ss[1])));
		
		TestSpdyMultiThread app = new TestSpdyMultiThread();
		app.startMonitor();
		for (int i = 0; i < nThreads; i++) {
			app.threadPool.execute(app.new TestTask(client, path));
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
