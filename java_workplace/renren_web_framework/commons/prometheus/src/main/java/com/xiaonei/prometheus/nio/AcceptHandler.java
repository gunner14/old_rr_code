package com.xiaonei.prometheus.nio;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.ServerSocketChannel;

import org.apache.log4j.Logger;

import com.xiaonei.prometheus.server.PrometheusServer;

public class AcceptHandler implements EventHandler{
	private CoreService core;
	private Logger log = Logger.getLogger(PrometheusServer.class);
	
	public AcceptHandler(CoreService core){
		this.core = core;
	}
	
	public void doHandler(SelectionKey key) throws IOException {
		if(key.isValid()&&key.isAcceptable()){
			onAccept(key);
		}
	}
	
	private void onAccept(SelectionKey key) throws IOException{
		ServerSocketChannel channel = (ServerSocketChannel)key.channel();
		log.debug(channel+" fire accept event");
		
		try {
			java.nio.channels.SocketChannel newConn = channel.accept();
			log.debug(newConn+" be accpet");
			newConn.configureBlocking(false);
			
			IProtocolHandlerFactory protocolHandlerFactory = core.getProtocolHandlerFactory();
			IProtocolHandler protocolHandler = protocolHandlerFactory.factory();
			
			SessionHandler sessionHandler = new SessionHandler(protocolHandler,core.getMessageHandler(),core.getWorkerThreadPool(),core.getOption().isEnableWorkerThread());
			sessionHandler.setSocketChannel(newConn);
			sessionHandler.setSelectionKey(key);
			
			core.getSelectorPool().register(newConn,SelectionKey.OP_READ, sessionHandler);
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
