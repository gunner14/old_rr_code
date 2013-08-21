package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.util.Set;

public class ReaderSelector	extends AbstractSelector {
	@Override
	public int getInterestOps() {
		return SelectionKey.OP_READ;
	}
	@Override
	public void register(Connection conn, int interestOps) {
		System.out.println("register reader connection");
		waitRegisterConnections.add(conn);
		selector.wakeup();
		System.out.println("register reader connection over");
	}
	public ReaderSelector() throws IOException{
		super();
	}
	@Override
	public void run() {
		while(true){
			handleRegister();
			
			try {
				selector.select();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			Set<SelectionKey> keys = selector.selectedKeys();
			if(keys.isEmpty())continue;
			for(SelectionKey key:keys){
				Connection conn = (Connection)key.attachment();
				if(!conn.isActive()){
					key.cancel();
					continue;
				}
				if(key.isReadable()){
					//System.out.println("readselecotr can read");
					try {
						conn.socketRead();
					} catch (IOException e) {
						key.cancel();
						conn.exception(e);
					}
				}
			}
			
			keys.clear();
		}
	}
}
