package com.xiaonei.prometheus.server;

import java.net.InetSocketAddress;
import com.xiaonei.prometheus.nio.CoreService;
import com.xiaonei.prometheus.nio.Option;

public class PrometheusServer {
	private java.net.InetSocketAddress address;
	public PrometheusServer(java.net.InetSocketAddress address){
		this.address = address;
	}
	
	public void start(){
		CoreService core = new CoreService(new Option(4));
		core.setProtoclHandlerFactory(new ProtocolHandlerFactory());
		
		core.addMessageHandler(new StatMessageHandler());
		core.addMessageHandler(new DefaultMessageHandler());
		
		core.bind(address);
	}
	
	public static void main(String[] args){
		if(args.length != 2){
			System.out.println("Usage:PrometheusServer <ip> <port>");
			System.exit(0);
		}
		
		String ip = args[0];
		int port = Integer.valueOf(args[1]);
		
		InetSocketAddress address = new InetSocketAddress(ip,port);
		PrometheusServer server = new PrometheusServer(address);
		server.start();
	}
}
