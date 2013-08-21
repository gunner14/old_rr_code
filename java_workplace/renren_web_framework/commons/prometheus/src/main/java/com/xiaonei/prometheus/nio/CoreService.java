package com.xiaonei.prometheus.nio;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.xiaonei.prometheus.LocalException;

public class CoreService {
	private Option option;
	private Selector acceptorThread;
	private SelectorPool threadPool;
	private MessageHandlerChain handlerChain = new MessageHandlerChain();
	private ExecutorService workerThread = Executors.newCachedThreadPool();
	private IProtocolHandlerFactory protocolHandlerFactory;
	private ConnectHandler connectHandler;
	private AcceptHandler acceptHandler;
	
	private volatile boolean started = false;
	
	public CoreService(Option option){
		this.option = option;
		acceptorThread = new Selector();
		threadPool = new SelectorPool(option.getSelectorPoolSize());
		connectHandler = new ConnectHandler(this);
		acceptHandler = new AcceptHandler(this);
	}
	
	public MessageHandler addMessageHandler(MessageHandler messageHandler){
		handlerChain.addHandler(messageHandler);
		return handlerChain;
	}
	
	public void setProtoclHandlerFactory(IProtocolHandlerFactory protocol){
		protocolHandlerFactory = protocol;
	}
	
	public Selector getAcceptorSelector(){
		return acceptorThread;
	}
	
	public SelectorPool getSelectorPool(){
		return threadPool;
	}
	
	public MessageHandler getMessageHandler(){
		return handlerChain;
	}
	
	public ExecutorService getWorkerThreadPool(){
		return workerThread;
	}
	
	public Option getOption(){
		return option;
	}
	
	public boolean isStarted(){
		return started;
	}
	
	public IProtocolHandlerFactory getProtocolHandlerFactory(){
		return protocolHandlerFactory;
	}
	
	public void start(){
		if(started)return;
		started = true;
		acceptorThread.start();
	}
	
	public void bind(java.net.InetSocketAddress address){
		try{
			ServerSocketChannel server = ServerSocketChannel.open();
			server.configureBlocking(false);
			java.net.ServerSocket socket = server.socket();
			socket.bind(address);
			acceptorThread.register(server, SelectionKey.OP_ACCEPT, acceptHandler);
			start();
		}catch(IOException ex){
			LocalException exception = new LocalException();
			exception.initCause(ex);
		}
	}
	
	public void connect(java.net.InetSocketAddress address,IConnectionHandler connection){
		try {
			SocketChannel socket = SocketChannel.open();
			socket.configureBlocking(false);
			
			connectHandler.addConnectionHandler(socket, connection);
			acceptorThread.register(socket, SelectionKey.OP_CONNECT, connectHandler);
			socket.connect(address);
			start();
		} catch (IOException e) {
			e.printStackTrace();
			throw new LocalException("");
		}
	}
}
