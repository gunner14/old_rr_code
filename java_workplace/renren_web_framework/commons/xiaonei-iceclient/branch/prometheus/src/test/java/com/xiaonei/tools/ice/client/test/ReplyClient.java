package com.xiaonei.tools.ice.client.test;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicLong;

import com.xiaonei.tools.ice.client.ice.Identity;


public class ReplyClient {

	public static class Stat {
		private AtomicLong _count = new AtomicLong(0);
		private static Stat _instance = new Stat();

		public static Stat getInstance() {
			return _instance;
		}

		private Stat() {
			_printer = new RatePrinter(_count);
			_printer.setDaemon(true);
			_printer.start();
		}

		public void inc() {
			_count.incrementAndGet();
		}

		private RatePrinter _printer;

		private static class RatePrinter extends Thread {
			private long _last;
			private AtomicLong _c;

			public RatePrinter(AtomicLong c) {
				_c = c;
			}

			@Override
			public void run() {
				while (true) {
					try {
						long current = _c.get();
						System.out.println("Rate: " + (current - _last)
								+ " req/s");
						_last = current;
						Thread.sleep(1000L);
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	public static class EchoThread extends Thread {
		private final com.xiaonei.tools.ice.client.IceClient _prx;
		private final int _in;

		public EchoThread(com.xiaonei.tools.ice.client.IceClient prx, int in, ThreadGroup group) {
			super(group, "EchoThread-" + in);
			_prx = prx;
			_in = in;
		}

		@Override
		public void run() {
			while (true) {
				try {
					Identity echoId = new Identity("M","");
					EchoOperation op1 = new EchoOperation(echoId,"echo");
					_prx.addOperation(op1);
					//Identity echoId = new Identity("M","");
					
					op1.getData();
					
					Stat.getInstance().inc();
				} catch (Throwable e) {
					e.printStackTrace();
				} finally {
				}
			}
		}
	}

	public static class DelayThread extends Thread {
		private final com.xiaonei.tools.ice.client.IceClient _prx;
		private final int _in;

		public DelayThread(com.xiaonei.tools.ice.client.IceClient prx, int in, ThreadGroup group) {
			super(group, "DelayThread-" + in);
			_prx = prx;
			_in = in;
		}

		@Override
		public void run() {
			while (true) {
				try {
					//_prx.delay(_in);
					Stat.getInstance().inc();
				} catch (Throwable e) {
					e.printStackTrace();
				} finally {
				}
			}
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if (args.length != 7) {
			System.out
					.println("Usage: ReplyClient <echo/delay> <ip> <port> <concurrent> <connections> <readselector> <writeselector>");
			System.exit(1);
		}

		String type = args[0];
		String ip = args[1];
		String port = args[2];
		
		Integer concurrent = Integer.valueOf(args[3]);
		Integer connections = Integer.valueOf(args[4]);
		Integer selectors = Integer.valueOf(args[5]);
		Integer wselectors = Integer.valueOf(args[6]);
		System.out.println("concurrent = " + concurrent);
		InetSocketAddress address = new InetSocketAddress(ip,Integer.parseInt(port));
		com.xiaonei.tools.ice.client.IceClient client = new com.xiaonei.tools.ice.client.IceClient(address,connections,selectors,wselectors);

		ThreadGroup group = new ThreadGroup("ReplyClient");
		List<Thread> threads = new ArrayList<Thread>();
		for (int x = 0; x < concurrent; ++x) {
			Thread t = type.equals("echo") ? new EchoThread(client, x, group)
					: new DelayThread(client, x, group);
			threads.add(t);
			t.setDaemon(false);
			t.start();
		}
	}

}
