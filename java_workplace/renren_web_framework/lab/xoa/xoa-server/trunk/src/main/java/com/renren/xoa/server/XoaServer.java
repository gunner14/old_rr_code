package com.renren.xoa.server;

import java.net.InetSocketAddress;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

import com.renren.xoa.commons.thread.NioThreadFactory;
import com.renren.xoa.server.coyote.ProtocolHandlerAdapter;
import com.renren.xoa.server.netty.http.XoaServerPipelineFactory;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 下午12:15:33
 */
public class XoaServer {

	private int port;
	
	private ServerContext serverContext = new ServerContext();
	
	private ServerBootstrap bootstrap;
	
	public XoaServer(ProtocolHandlerAdapter protocolHandlerAdapter, Executor executor, int port) {
		this.port = port;
		serverContext.setProtocolHandlerAdapter(protocolHandlerAdapter);
		serverContext.setPort(port);
		if(executor == null) {
		    throw new IllegalArgumentException("You must specify an executor.");
		}
		serverContext.setExecutor(executor);
	}
	
	public void start() {
		
		System.out.println("starting xoa-server at port:" + port);
		
		NioThreadFactory threadFacotry = new NioThreadFactory();
		// Configure the server.
        ServerBootstrap bootstrap = new ServerBootstrap(new NioServerSocketChannelFactory(Executors
                .newCachedThreadPool(threadFacotry), Executors.newCachedThreadPool(threadFacotry)));
        bootstrap.setOption("child.tcpNoDelay", true);
        bootstrap.setOption("child.keepAlive", true);
        bootstrap.setOption("child.connectTimeoutMillis", 1000);
		// Set up the event pipeline factory.
        XoaServerPipelineFactory fact = new XoaServerPipelineFactory();
        fact.init(serverContext);
		bootstrap.setPipelineFactory(fact);

		// Bind and start to accept incoming connections.
		bootstrap.bind(new InetSocketAddress(port));
		this.bootstrap = bootstrap;
		try {
			serverContext.getExecutor().getClass().getMethod("start").invoke(
					serverContext.getExecutor());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void stop() {
		bootstrap.releaseExternalResources();
		try {
			serverContext.getExecutor().getClass().getMethod("stop").invoke(
					serverContext.getExecutor());
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		//延迟退出
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
