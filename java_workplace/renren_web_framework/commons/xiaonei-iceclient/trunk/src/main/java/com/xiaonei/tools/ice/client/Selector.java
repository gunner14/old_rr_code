package com.xiaonei.tools.ice.client;

public interface Selector extends Runnable {
	public void addConnection(Connection conn);
	public void register(Connection conn,int ops);
	//public void wakeup();
}
