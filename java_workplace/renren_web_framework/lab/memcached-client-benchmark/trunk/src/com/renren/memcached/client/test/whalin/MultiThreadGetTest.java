package com.renren.memcached.client.test.whalin;

import java.util.Map;

import com.danga.MemCached.MemCachedClient;

public class MultiThreadGetTest {

	public static void main(String[] args) {

		if (args.length != 4) {
			System.err
					.println("Usage:java MultiThreadGetTest nodes keys threads loops");
			return;
		}

		startTest(Integer.parseInt(args[0]), Integer.parseInt(args[1]), Integer
				.parseInt(args[2]), Integer.parseInt(args[3]));

	}

	private static void startTest(int nNodes, final int nKeys, int nThreads,
			final int loops) {

		final MemCachedClient client = MemcachedClientFactory.getClient(nNodes);

		// 初始化需要用到的key
		initKeys(client, nKeys);

		// 线程里并发取所有的key
		final String[] keys = genKeys(0, nKeys);
		
		// 开线程
		Thread[] threads = new Thread[nThreads];
		for (int i = 0; i < threads.length; i++) {
			threads[i] = new Thread(new Runnable() {
				@Override
				public void run() {
					int n = 0;
					String threadName = Thread.currentThread().getName();
					while (n++ < loops) { // 循环操作数次
						if (n % 2000 == 0) {
							System.out.println(n + "-th loop of " + threadName);
						}
						try {
							Map<String, Object> ret = client.getMulti(keys);
							if (ret.size() != keys.length) {
								System.err.println("result set size:"
										+ ret.size() + ", but key set size:"
										+ keys.length);
							}
						} catch (Exception e) {
							e.printStackTrace();
						}
					}
				}
			});
		}
		
		System.out.println("start multi-get benchmark");
		long start = System.currentTimeMillis();
		for (Thread t : threads) {
			t.start();
		}
		for (Thread t : threads) {
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		long end = System.currentTimeMillis();
		long cost = end - start;
		long count = nThreads * loops;
		double per = cost * 1.0 / (count);
		System.out.println(count + " multi-get benchmark done, cost " + cost + "ms, " + per + "ms per request.");
	}

	private static void initKeys(MemCachedClient client, int keys) {

		System.out.println("start to init keys...");
		long start = System.currentTimeMillis();

		for (int i = 0; i < keys; i++) {
			client.set(genKey(i), data);
		}
		long end = System.currentTimeMillis();
		System.out.println(keys + " keys initialized, cost " + (end - start)
				+ "ms");

	}

	private static final String data = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
	private static final String key_prefix = "key";

	private static String genKey(int n) {
		return key_prefix + n;
	}

	private static String[] genKeys(int start, int end) {

		String[] ret = new String[end - start];
		for (int i = 0; i < ret.length; i++) {
			ret[i] = key_prefix + (i + start);
		}
		return ret;
	}

}
