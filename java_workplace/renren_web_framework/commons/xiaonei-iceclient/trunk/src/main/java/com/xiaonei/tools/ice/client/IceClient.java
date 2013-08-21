package com.xiaonei.tools.ice.client;

import java.nio.channels.SelectionKey;

public class IceClient {
	private ConnectionFactory connFactory = new ConnectionFactory();
	private java.net.InetSocketAddress address;
	private int connSize;
	private int selectorSize;
	private static int defaultConnSize = 2;
	private static int defaultRSize = 1; 
	private static int defaultWSize = 1;
	private SelectorPool selectorPool;
	public IceClient(java.net.InetSocketAddress address){
		this(address,defaultConnSize,defaultRSize,defaultWSize);
	}
	public IceClient(java.net.InetSocketAddress address,int connSize,int rSize,int wSize){
		this.address = address;
		if(selectorSize > connSize){
			selectorSize = connSize;
		}
		this.connSize = connSize;
		
		selectorPool = new SelectorPool(rSize,wSize);
		
		for(int i=0;i<this.connSize;i++){
			Connection conn = connFactory.createConnection(address);
			Selector selector  =  selectorPool.getReaderSelector();
			conn.register(selector, SelectionKey.OP_READ);
			selector = selectorPool.getWriterSelector();
			conn.register(selector, SelectionKey.OP_WRITE);
			conn.waitRegisterComplete();
		}
		
		System.out.println("Client Init Over");
	}
	
	public void addOperation(Operation op){
		Connection conn = connFactory.getConnection(address);
		conn.addOperation(op);
	}
	
}
