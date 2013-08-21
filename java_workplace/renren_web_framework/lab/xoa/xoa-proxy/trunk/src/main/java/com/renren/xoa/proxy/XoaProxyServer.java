package com.renren.xoa.proxy;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

import com.renren.xoa.proxy.pipeline.XoaProxyPipelineFactory;

public class XoaProxyServer {

	private int port;
	
	private ServerBootstrap bootstrap;

	public XoaProxyServer(int port) {
		this.port = port;
	}

	public void start() {

		System.out.println("starting xoa-proxy at port:" + port);
		// Configure the server.
		ServerBootstrap bootstrap = new ServerBootstrap(
				new NioServerSocketChannelFactory(Executors
						.newCachedThreadPool(), Executors.newCachedThreadPool()));
		bootstrap.setOption("child.tcpNoDelay", true);
		bootstrap.setOption("child.keepAlive", true);
		bootstrap.setOption("child.connectTimeoutMillis", 1000);
		// Set up the event pipeline factory.
		ChannelPipelineFactory fact = new XoaProxyPipelineFactory();
		bootstrap.setPipelineFactory(fact);
		// Bind and start to accept incoming connections.
		bootstrap.bind(new InetSocketAddress(port));
		this.bootstrap = bootstrap;
	}

	public static void main(String[] args) {
		if (args.length != 2) {
			System.err.println("Usage:java XoaProxyServer port backend");
			return;
		}
		BackendManager.getInstance().setBackend(args[1]);
		new XoaProxyServer(Integer.parseInt(args[0])).start();
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
