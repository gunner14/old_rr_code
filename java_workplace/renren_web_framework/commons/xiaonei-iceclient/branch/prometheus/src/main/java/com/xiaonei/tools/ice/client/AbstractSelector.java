package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentLinkedQueue;

public abstract class AbstractSelector implements Selector {
	protected java.nio.channels.Selector selector;
	protected ConcurrentLinkedQueue<Connection> waitRegisterConnections = new ConcurrentLinkedQueue<Connection>();
	
	public AbstractSelector() throws IOException{
		this.selector = java.nio.channels.Selector.open();
	}
	protected void handleRegister(){
		while(waitRegisterConnections.size() > 0){
			Connection conn = waitRegisterConnections.poll();
			if(conn==null)continue;
			SocketChannel channel = conn.getSocket();
			try {
				SelectionKey key = channel.register(selector, getInterestOps(), conn);
				if(getInterestOps()==SelectionKey.OP_WRITE){
					conn.setWriterSeletionKey(key);
				}
				conn.registerCallback();
			} catch (ClosedChannelException e) {
				e.printStackTrace();
			}
		}
	}
	
	@Override
	public void addConnection(Connection conn) {
		throw new java.lang.UnsupportedOperationException();
	}
	public abstract int getInterestOps();
	@Override
	public abstract void register(Connection conn, int interestOps);
	
	public abstract void run();

}
