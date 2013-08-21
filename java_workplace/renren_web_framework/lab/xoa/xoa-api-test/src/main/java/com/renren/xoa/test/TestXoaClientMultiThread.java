package com.renren.xoa.test;

import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import com.renren.xoa.XoaClient;
import com.renren.xoa.Method;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList;
import com.renren.xoa.methods.XoaGetMethod;
import com.renren.xoa.registry.XoaRegistry;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;


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

		private String serviceId;
		
		private String path;
		
		private XoaClient client;
		
		public TestTask(String domain, String path, XoaRegistry registry) {
			this.serviceId = domain;
			this.path = path;
			client = new XoaClient(registry);
		}
		
		@Override
		public void run() {
			while (running.get()) {
				try {
					Method method = Method.get(serviceId + path);
					long startTime = System.currentTimeMillis();
					Future<XoaResponse> f = client.submit(method);
					sent.incrementAndGet();
					XoaResponse res = f.get();
					String contentType = res.getHeader("Content-Type");
					if (contentType.contains("application/json")) {
						res.getContentAs(Blog[].class);
					} else if(contentType.contains("application/protoc-buff")){
						res.getContentAs(RizhiList.class);
					} else if (contentType.contains("application/serializable")) {
						res.getContentAs(Blog[].class);
					} else {
						res.getContentAsString();
					}
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
	
	private static void multiThreadTest(String host, String path, int nThreads) {
		
		System.out.println("start xoa-client multi-thread test");
		
		String servicdId = "blog.xoa.renren.com";
		
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		desc.setIpAddress(host.substring(0, host.indexOf(':')));
		desc.setPort(Integer.parseInt(host.substring(host.indexOf(':') + 1)));
		reg.register(desc);
		
		/*String domain = "10.3.21.36:8888";
		String path = "/spdy/index.jsp";*/
		TestXoaClientMultiThread app = new TestXoaClientMultiThread();
		app.startMonitor();
		for (int i = 0; i < nThreads; i++) {
			app.threadPool.execute(app.new TestTask(servicdId, path, reg));
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
