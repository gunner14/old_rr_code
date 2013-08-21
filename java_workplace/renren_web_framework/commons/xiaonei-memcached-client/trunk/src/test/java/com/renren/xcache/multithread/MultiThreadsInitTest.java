package com.renren.xcache.multithread;

import java.util.Random;
import java.util.concurrent.CountDownLatch;

import org.junit.Test;

import com.renren.xcache.XCache;
import com.renren.xcache.model.User;

public class MultiThreadsInitTest {

	@Test
	public void go() {
		int nThreads = 20;
		
		User[] users = genUsers(20);
		final String[] ids = new String[users.length];
		
		for (int i = 0; i < users.length; i++) {
			String id = "" + users[i].getId();
			ids[i] = id;
			//cache.set(id, users[i], XCache.EXPIRE_SECOND * 10);
		}
		
		final CountDownLatch latch = new CountDownLatch(1);
		
		final CountDownLatch finishLatch = new CountDownLatch(nThreads);
		
		Thread[] threads = new Thread[nThreads];
		for (int i = 0; i < threads.length; i++) {
			threads[i] = new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						latch.await();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					XCache<User> cache = XCache.getCache("test.user", User.class);
					cache.getMulti(ids);
					finishLatch.countDown();
				}
			});
		}
		for (int i = 0; i < threads.length; i++) {
			threads[i].start();
		}
		System.out.println("Threads are ready. Go!");
		latch.countDown();
		
		try {
			finishLatch.await();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	private User[] genUsers(int n) {
		Random ran = new Random();
		User[] users = new User[n];
		for (int i = 0; i < users.length; i++) {
			users[i] = new User(i, "n" + ran.nextInt(1000), ran.nextInt(100));
		}
		return users;
	}
	
}
