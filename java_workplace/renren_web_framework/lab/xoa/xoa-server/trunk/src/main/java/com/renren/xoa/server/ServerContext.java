package com.renren.xoa.server;

import java.util.concurrent.Executor;

import com.renren.xoa.server.coyote.ProtocolHandlerAdapter;

/**
 * 服务器上下文变量
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 下午12:00:57
 */
public class ServerContext {

	private ProtocolHandlerAdapter protocolHandlerAdapter;

	private int port;
	
	private Executor executor;
	
	public ProtocolHandlerAdapter getProtocolHandlerAdapter() {
		return protocolHandlerAdapter;
	}

	public void setProtocolHandlerAdapter(
			ProtocolHandlerAdapter protocolHandlerAdapter) {
		this.protocolHandlerAdapter = protocolHandlerAdapter;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public Executor getExecutor() {
		return executor;
	}

	public void setExecutor(Executor executor) {
		this.executor = executor;
	}
	
	
}
