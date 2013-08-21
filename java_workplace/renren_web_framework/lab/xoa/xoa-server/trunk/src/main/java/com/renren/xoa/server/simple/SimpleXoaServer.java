package com.renren.xoa.server.simple;

import java.net.InetSocketAddress;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.apache.catalina.LifecycleException;
import org.apache.catalina.core.StandardThreadExecutor;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

import com.renren.xoa.server.simple.pipeline.SimpleXoaServerPipelineFactory;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 下午12:15:33
 */
public class SimpleXoaServer {

	private int port;

	private ServerBootstrap bootstrap;

	private Executor executor;

	public SimpleXoaServer(int port, int userThreadPool) {
		this.port = port;
		if (userThreadPool == 1) {
			executor = getExecutor();
		}
	}

	public static Executor getExecutor() {
		StandardThreadExecutor executor = new StandardThreadExecutor();
		executor.setDaemon(true);
		executor.setMaxThreads(50);
		executor.setMinSpareThreads(2);
		executor.setNamePrefix("xoa-exec");
		try {
			executor.start();
		} catch (LifecycleException e) {
			e.printStackTrace();
		}

		return executor;
	}

	public void start() {

		System.out.println("starting simple-xoa-server at port:" + port);
		// Configure the server.
		ServerBootstrap bootstrap = new ServerBootstrap(
				new NioServerSocketChannelFactory(Executors
						.newCachedThreadPool(), Executors.newCachedThreadPool()));
		bootstrap.setOption("child.tcpNoDelay", true);
		bootstrap.setOption("child.keepAlive", false);
		bootstrap.setOption("child.connectTimeoutMillis", 1000);
		// Set up the event pipeline factory.
		ChannelPipelineFactory fact = new SimpleXoaServerPipelineFactory()
				.setExecutor(executor);
		bootstrap.setPipelineFactory(fact);
		// Bind and start to accept incoming connections.
		bootstrap.bind(new InetSocketAddress(port));
		this.bootstrap = bootstrap;
	}

	/*public static void main(String[] args) {
		Executor exe = getExecutor();
		int submitCount = 0;
		final AtomicInteger handledCount = new AtomicInteger();
		long startTime = System.currentTimeMillis();
		
		int max = 5000000;
		
		while (handledCount.get() < max) {
			
			if (submitCount >= max) {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			
			exe.execute(new Runnable() {
				@Override
				public void run() {
					
					int streamId = handledCount.get();
					
					SpdyHttpResponse nettyResponse = new SpdyHttpResponse(HttpVersion.HTTP_1_1,
							HttpResponseStatus.valueOf(200),streamId);
					String responseContent = TextFactory.getInstance().getString(0);
					//ChannelBuffer buff = ChannelBuffers.dynamicBuffer(responseContent.length() + 10);
					ChannelBuffer buff = ChannelBuffers.EMPTY_BUFFER;
					//buff.writeBytes(responseContent.getBytes());
					nettyResponse.setContent(buff);
					
					handledCount.incrementAndGet();
				}
			});
			submitCount++;
			if (submitCount % 100000 == 0) {
				System.out.println("submit:" + submitCount + ", handled:"
						+ handledCount.get());
			}
		}
		long endTime = System.currentTimeMillis();

		int duration = (int) (endTime - startTime) / 1000;

		System.out.println("submit:" + 1.0 * submitCount / duration
				+ "/s, handled:" + handledCount.get() * 1.0 / duration + "/s");
	}*/

	public static void main(String[] args) {
		if (args.length != 2) {
			System.err.println("Usage:java xxx port useThreadPool");
			return;
		}
		new SimpleXoaServer(Integer.parseInt(args[0]), Integer
				.parseInt(args[1])).start();
		
	}

	public void stop() {
		bootstrap.releaseExternalResources();

		// 延迟退出
		new Thread(new Runnable() {

			@Override
			public void run() {
				try {
					Thread.sleep(1500);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.exit(0);
			}
		}).start();
	}
}
