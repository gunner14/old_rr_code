package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionFactory {
	private static int bufferSize = 1024;
	private Map<InetSocketAddress,AtomicInteger> connRobin = new HashMap<InetSocketAddress,AtomicInteger>(); 
	private Map<InetSocketAddress,ArrayList<Connection>> connections = new HashMap<InetSocketAddress,ArrayList<Connection>>();
	public ConnectionFactory(){
	
	}
	public Connection createConnection(InetSocketAddress address){
		try{
			System.out.println("createConnection");
			SocketChannel fd = SocketChannel.open();
			java.net.Socket socket = fd.socket();
			socket.setTcpNoDelay(true);
			socket.setKeepAlive(true);
			fd.configureBlocking(false);
			//System.out.println("Connect Start");
			if(!fd.connect(address)){
				Selector selector = Selector.open();
				try{
					fd.register(selector, java.nio.channels.SelectionKey.OP_CONNECT);
					int n = selector.select();
					if(n==0)throw new IOException("ConnectTimeout");
					if(!fd.finishConnect())throw new IOException("ConnectFail");
				}finally{
					selector.close();
				}
			}
			//System.out.println("Connection OK");
			Connection conn = new Connection(address,fd,bufferSize);
			ArrayList<Connection> list = connections.get(address);
			if(list==null){
				list = new ArrayList<Connection>();
				connRobin.put(address, new AtomicInteger(0));
				connections.put(address, list);
			}
			list.add(conn);
			return conn;
			
		}catch(IOException e){
			RuntimeException er = new RuntimeException();
			er.initCause(e);
			throw er;
		}
	}
	
	public Connection getConnection(InetSocketAddress address){
		ArrayList<Connection> conns = connections.get(address);
		if(conns==null&&conns.size()==0)return null;
		AtomicInteger robinNext = connRobin.get(address);
		int next = robinNext.getAndIncrement();
		Connection conn = conns.get(next%conns.size());
		return conn;
	}
}
