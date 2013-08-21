package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Selector implements Runnable{
	private java.nio.channels.Selector selector;
	private volatile boolean needWakeup = true;
	private volatile boolean change = true;
	//private volatile boolean 
	private ConcurrentLinkedQueue<Connection> addedQueue = new ConcurrentLinkedQueue<Connection>();
	private ConcurrentLinkedQueue<Connection> registerConns = new ConcurrentLinkedQueue<Connection>();
	public Selector() throws IOException{
		selector = java.nio.channels.Selector.open();
	}
	
	public void register(Connection conn,int interestOps){
		System.out.println("register connection");
		conn.setInterestOps(interestOps);
		conn.setSelector(this);
		registerConns.add(conn);
		selector.wakeup();
		System.out.println("register connection over");
	}
	
	public void addConnection(Connection conn){
		addedQueue.add(conn);
	}
	
	public void wakeup(){
		if(needWakeup){
			selector.wakeup();
		}else{
			//System.out.println("not need wakeup");
		}
	}
	
	private void handleRegister(){
		while(registerConns.size() > 0){
			Connection conn = registerConns.poll();
			if(conn==null)continue;
			SocketChannel channel = conn.getSocket();
			try {
				SelectionKey key = channel.register(selector, conn.getInterestOps(), conn);
				System.out.println("register "+conn.getInterestOps());
				conn.setSelectionKey(key);
				conn.registerCallback();
			} catch (ClosedChannelException e) {
				e.printStackTrace();
			}
		}
	}
	
	private void handleQueue(){
		if(addedQueue.isEmpty())return;
		Collection<Connection> events = new HashSet<Connection>();
		Connection conn = null;
		while((conn=addedQueue.poll())!=null){
			events.add(conn);
		}
		
		for(Connection connection:events){
			if(connection.getNeedWriteBytes()>0){
				try {
					connection.socketWrite();
				} catch (IOException e) {
					connection.exception(e);
				}
			}
			connection.fixupOps();
		}
	}
	
	public void run(){
		while(true){
			handleRegister();
			handleQueue();
			
			try{
				selector.select();
			}catch(IOException ex){
				ex.printStackTrace();
			}
			needWakeup = false;
			Set<SelectionKey> keys = selector.selectedKeys();
			
			Iterator<SelectionKey> keysIter = keys.iterator();
			
			while(keysIter.hasNext()){
				SelectionKey key = keysIter.next();
				Connection conn = (Connection)key.attachment();
				if(conn.isActive()){
					if(key.isReadable()){
						try{
							conn.socketRead();
						}catch(IOException ex){
							key.cancel();
							conn.exception(ex);
						}
					}
					
					if(key.isWritable()){
						try {
							conn.socketWrite();
						} catch (IOException ex) {
							key.cancel();
							conn.exception(ex);
						}
					}
				}
				conn.fixupOps();
			}
			boolean needWrite = false;
			for(SelectionKey key:selector.keys()){
				if((key.interestOps() & ~SelectionKey.OP_READ)!=SelectionKey.OP_WRITE){
					needWrite = true;
				}
			}
			needWakeup = needWrite;
			keys.clear();
		}
	}
}
