package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.ConcurrentLinkedQueue;

public class WriterSelector extends AbstractSelector {
	@Override
	public int getInterestOps() {
		return SelectionKey.OP_WRITE;
	}

	@Override
	public void register(Connection conn, int interestOps) {
		System.out.println("register writer connection");
		waitRegisterConnections.add(conn);
		conn.setWriterSelector(this);
		selector.wakeup();
		System.out.println("register writer connection over");
	}

	@Override
	public void addConnection(Connection conn) {
		addedConnection.add(conn);
		if(needWakeup){
			selector.wakeup();
		}else{
			//System.out.println("not need wakeup");
		}
	}
	
	private void handleQueue(){
		if(addedConnection.isEmpty())return;
		Collection<Connection> events = new HashSet<Connection>();
		Connection conn = null;
		while((conn=addedConnection.poll())!=null){
			events.add(conn);
		}
		
		for(Connection connection:events){
			connection.fixupOps();
		}
	}
	
	private ConcurrentLinkedQueue<Connection> addedConnection = new ConcurrentLinkedQueue<Connection>();
	private volatile boolean needWakeup = false;
	public WriterSelector() throws IOException{
		super();
	}
	
	@Override
	public void run() {
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
			if(keys.isEmpty()){
				for(SelectionKey key : selector.keys()){
					if(key.interestOps()!=SelectionKey.OP_WRITE){
						needWakeup = true;
						break;
					}
				}
				continue;
			}
			for(SelectionKey key:keys){
				Connection conn = (Connection)key.attachment();
				if(!conn.isActive()){
					key.cancel();
					continue;
				}
				if(key.isWritable()){
					try {
						conn.socketWrite();
					} catch (IOException e) {
						key.cancel();
						conn.exception(e);
					}
				}
				if(!needWakeup && conn.fixupOps()!=SelectionKey.OP_WRITE){
					needWakeup = true;
				}
			}
			
			keys.clear();
		}
	}

}
