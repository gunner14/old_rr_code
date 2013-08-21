package com.renren.memcached.client.test;

import java.util.ArrayList;
import java.util.List;

public class Server {

	private String host;
	
	private int port;

	public Server(String hostport) {
		String[] ss = hostport.split(":");
		host = ss[0];
		port = Integer.parseInt(ss[1]);
	}
	
	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}
	
	public static void main(String[] args) {
		
		List<String> ss = new ArrayList<String>();
		ss.add("10.22.200.138");
		ss.add("10.22.200.140");
		ss.add("10.22.200.65");
		ss.add("10.22.200.66");
		ss.add("10.22.200.68");
		ss.add("10.22.200.69");
		ss.add("10.22.206.157");
		ss.add("10.22.202.34");
		
		
		for (int p = 11211; p <= 11218; p++) {
			for (String s : ss) {
				System.out.println(s + ":" + p);
			}
		}
		
		
	}
}
