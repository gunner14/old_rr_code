package com.xiaonei.prometheus.benchmark;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicLong;

import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.nio.CoreService;
import com.xiaonei.prometheus.nio.Option;
import com.xiaonei.prometheus.server.ConnectionHandler;
import com.xiaonei.prometheus.server.Message;
import com.xiaonei.prometheus.server.Protocol;
import com.xiaonei.prometheus.server.ProtocolHandlerFactory;

public class EchoBenchmark {

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
		private final ConnectionHandler connection;
		private final int _in;

		public EchoThread(ConnectionHandler prx, int in, ThreadGroup group) {
			super(group, "EchoThread-" + in);
			connection = prx;
			_in = in;
		}

		@Override
		public void run() {
			int i = 0;
			while (true) {
				try {
					i++;
					
					Buffer buffer = new Buffer();
					String args = "prometheus come to renren.com";
					buffer.writeByte(args.getBytes());
					
					Message send = new Message();
					send.setSeqId(i);
					send.setOpcode(Protocol.OPCODE_GET);
					send.setData(buffer);
					
					Message data = connection.syncWrite(send);
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
		if (args.length != 5) {
			System.out
					.println("Usage: EchoBenchmark <ip> <port> <concurrent> <connections> <readselector>");
			System.exit(1);
		}

		String ip = args[0];
		String port = args[1];
		
		Integer concurrent = Integer.valueOf(args[2]);
		Integer connections = Integer.valueOf(args[3]);
		Integer selectors = Integer.valueOf(args[4]);
		System.out.println("concurrent = " + concurrent);
		InetSocketAddress address = new InetSocketAddress(ip,Integer.parseInt(port));
		
		CoreService core = new CoreService(new Option(selectors));
		core.setProtoclHandlerFactory(new ProtocolHandlerFactory());
		
		ConnectionHandler[] handlers = new ConnectionHandler[connections];
		for(int i=0;i<connections;i++){
			handlers[i] = new ConnectionHandler();
			core.connect(address, handlers[i]);
		}
		
		ThreadGroup group = new ThreadGroup("EchoBenchmark");
		List<Thread> threads = new ArrayList<Thread>();
		for (int x = 0; x < concurrent; ++x) {
			Thread t = new EchoThread(handlers[x%connections], x, group);
			threads.add(t);
			t.setDaemon(false);
			t.start();
		}
	}

}
