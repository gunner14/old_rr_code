package com.xiaonei.platform.core.cache;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import net.spy.memcached.KetamaConnectionFactory;
import net.spy.memcached.MemcachedClient;

public class Benchmark {
	public static void main(String[] args) throws IOException {
		if (args.length != 4) {
			System.out
					.println("Benchmark Uages:<host> <port> <clientsize> <threadsize>");
			System.exit(0);
		}

		String host = args[0];
		int port = Integer.parseInt(args[1]);
		int size = Integer.parseInt(args[2]);
		int tsize = Integer.parseInt(args[3]);
		Client client = new Client(host, port, size);
		Stat stat = new Stat();
		stat.start();

		for (int i = 0; i < tsize; i++) {
			Thread t = new Thread(new EchoThread(client, stat));
			t.start();
		}

	}
	
	
	public static class EchoThread implements Runnable {
		private Client client;
		private Stat stat;

		private EchoThread(Client client, Stat stat) {
			this.client = client;
			this.stat = stat;
		}

		@Override
		public void run() {
			while (true) {
				try {
					client.get();
				} catch (Exception e) {
					e.printStackTrace();
				}
				stat.inc();
			}
		}
	}

	public static class Stat {
		private int last;
		private AtomicInteger counter = new AtomicInteger(0);
		private ScheduledExecutorService scheduler = Executors
				.newScheduledThreadPool(1);

		public Stat() {

		}

		public void inc() {
			counter.incrementAndGet();
		}

		public void start() {

			Runnable print = new Runnable() {
				public void run() {
					int current = counter.get();
					System.out.println("Rate: " + (current - last) + " req/s");
					last = current;
				}
			};

			scheduler.scheduleWithFixedDelay(print, 0, 1, TimeUnit.SECONDS);
		}
	}

	public static class Client {
		private MemcachedClient[] caches;
		// private static String value = "benchmark"
		private AtomicInteger next = new AtomicInteger(0);
		private Random random = new Random();

		public Client(String host, int port, int size) throws IOException {
			caches = new MemcachedClient[size];
			List<InetSocketAddress> list = new ArrayList<InetSocketAddress>();
			list.add(new InetSocketAddress(host, port));
			for (int i = 0; i < size; i++) {
				MemcachedClient c = new MemcachedClient(
						new KetamaConnectionFactory(), list);
				caches[i] = c;
			}
		}

		public void get() throws Exception{
			/*int index = next.incrementAndGet();
			MemcachedClient c = caches[index % caches.length];
			String key = "test_" + random.nextInt(1000);
			OperationFuture<Boolean> setOp = c.set(key, 10, "benchmark");
			boolean succ = setOp.get(200, TimeUnit.MILLISECONDS);
			if (!succ) {
				System.out.println("set failed");
			}
			
			GetFuture<Object> getOp = c.asyncGet(key);
			String value = (String)getOp.get(200, TimeUnit.MILLISECONDS);
			if (!"benchmark".equals(value)) {
				System.out.println("e");
			}*/
		}
	}
}
