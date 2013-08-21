package com.xiaonei.tools.ice.client;

import java.nio.channels.SelectionKey;

public class IceClient {
	private ConnectionFactory connFactory = new ConnectionFactory();
	private java.net.InetSocketAddress address;
	private int connSize;
	private int selectorSize;
	private static int defaultConnSize = 10;
	private static int defaultSelectorSize = 5; 
	private SelectorPool selectorPool;
	public IceClient(java.net.InetSocketAddress address){
		this(address,defaultConnSize,defaultSelectorSize);
	}
	public IceClient(java.net.InetSocketAddress address,int connSize,int selectorSize){
		this.address = address;
		if(selectorSize > connSize){
			selectorSize = connSize;
		}
		this.connSize = connSize;
		this.selectorSize = selectorSize;
		
		selectorPool = new SelectorPool(this.selectorSize);
		
		for(int i=0;i<this.connSize;i++){
			Connection conn = connFactory.createConnection(address);
			Selector selector  =  selectorPool.getSelector();
			conn.register(selector, SelectionKey.OP_READ|SelectionKey.OP_WRITE);
		}
		System.out.println("Client Init Over");
	}
	
	public void addOperation(Operation op){
		Connection conn = connFactory.getConnection(address);
		//System.out.println("getConnection");
		conn.addOperation(op);
	}
	
}
