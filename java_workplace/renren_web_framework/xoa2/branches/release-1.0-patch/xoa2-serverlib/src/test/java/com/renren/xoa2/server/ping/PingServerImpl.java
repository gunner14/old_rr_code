package com.renren.xoa2.server.ping;

public class PingServerImpl implements IPingServer {
	int counter;
	
	@Override
	public int ping() {
		++counter;
		
		System.out.println("PingServer: " + counter + " ping received!");

		return counter;
	}
}
