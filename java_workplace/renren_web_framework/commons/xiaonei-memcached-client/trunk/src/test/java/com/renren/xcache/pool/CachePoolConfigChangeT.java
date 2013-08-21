package com.renren.xcache.pool;

import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import com.renren.xcache.XCache;


public class CachePoolConfigChangeT {

	private Random random = new Random();
	
	private AtomicInteger hitCount = new AtomicInteger();
	
	private AtomicInteger missCount = new AtomicInteger();
	
	ScheduledExecutorService exe = Executors.newSingleThreadScheduledExecutor();
	
	public CachePoolConfigChangeT() {
		startMonitor();
	}
	
	private void startMonitor() {
		exe.scheduleAtFixedRate(new Runnable() {
			
			@Override
			public void run() {
				System.out.println("hit:" + hitCount.get() + ", miss:" + missCount.get());
			}
		}, 0, 10, TimeUnit.SECONDS);
	}

	void runTest(int nThread) {
		
		for (int i = 0; i < nThread; i++) {
			Thread t = new Thread(new Runnable() {
				
				XCache<Integer> cache = XCache.getCache("test.test", Integer.class);
				
				@Override
				public void run() {
					while (true) {
						int id = random.nextInt(2000);
						Object ret = cache.get(String.valueOf(id));
						if (ret != null) {
							hitCount.incrementAndGet();
						} else {
							missCount.incrementAndGet();
						}
						try {
							Thread.sleep(100);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			});
			t.start();
		}
	}
	
	public static void main(String[] args) {
		new CachePoolConfigChangeT().runTest(10);
	}
}
