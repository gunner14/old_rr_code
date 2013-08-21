package com.xiaonei.prometheus.nio;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.log4j.Logger;

import com.xiaonei.prometheus.LocalException;
import com.xiaonei.prometheus.server.PrometheusServer;

public class ConnectHandler implements EventHandler{
	private ConcurrentHashMap<SocketChannel, IConnectionHandler> connectionMap = new ConcurrentHashMap<SocketChannel, IConnectionHandler>();
	private Logger log = Logger.getLogger(PrometheusServer.class);
	
	private CoreService core;
	public ConnectHandler(CoreService core){
		this.core = core;
	}
	
	public void addConnectionHandler(SocketChannel socket,IConnectionHandler connection){
		connectionMap.put(socket, connection);
	}
	
	public void doHandler(SelectionKey key) throws IOException {
		if(key.isValid()&&key.isConnectable()){
			onConnect(key);
		}
	}
	
	private void onConnect(SelectionKey key) throws IOException{
		SocketChannel channel = (SocketChannel)key.channel();
		log.debug(channel+" fire connect event");
		if(!channel.finishConnect()){
			throw new LocalException("not finishConnect");
		}
		IConnectionHandler connection = connectionMap.get(channel);
		if(connection == null){
			throw new IOException("connection map error");
		}
		
		IProtocolHandlerFactory protocolHandlerFactory = core.getProtocolHandlerFactory();
		IProtocolHandler protocolHandler = protocolHandlerFactory.factory();
		
		SessionHandler session = new SessionHandler(protocolHandler,connection,core.getWorkerThreadPool(),core.getOption().isEnableWorkerThread());
		session.setSocketChannel(channel);
		session.setSelectionKey(key);
		
		connection.setSessionHandler(session);
		connection.setProtocolHandler(protocolHandler);
		connection.completeConnect();
		
		core.getSelectorPool().register(channel, SelectionKey.OP_READ, session);
	}
}
