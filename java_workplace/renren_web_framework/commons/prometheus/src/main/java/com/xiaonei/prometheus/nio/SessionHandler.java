package com.xiaonei.prometheus.nio;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ExecutorService;
import org.apache.log4j.Logger;

import com.xiaonei.prometheus.server.PrometheusServer;

public class SessionHandler implements EventHandler{
	private MessageHandler handlerChain;
	private ByteBuffer buffer;
	private IProtocolHandler protocolHandler;
	private boolean enableWorkerThread = true;
	private int bufferSize = 1024;//need configure
	private ExecutorService executor;
	private java.nio.channels.SocketChannel socketChannel;
	private java.nio.channels.SelectionKey selectionKey;
	private volatile boolean hasWaitWriteQueue = false;
	private ConcurrentLinkedQueue<ByteBuffer> waitQueue = new ConcurrentLinkedQueue<ByteBuffer>();
	
	private Logger log = Logger.getLogger(PrometheusServer.class);

	public SessionHandler(IProtocolHandler protocolHandler){
		this.protocolHandler = protocolHandler;
		buffer = ByteBuffer.allocate(bufferSize);
	}
	
	public IProtocolHandler getProtocolHandler(){
		return this.protocolHandler;
	}
	
	public SessionHandler(IProtocolHandler protocolHandler,MessageHandler chain,ExecutorService executor,boolean enableWorkerThread){
		this(protocolHandler);
		this.handlerChain = chain;
		this.executor = executor;
		this.enableWorkerThread = enableWorkerThread;
	}
	
	public void setSocketChannel(SocketChannel channel){
		this.socketChannel = channel;
	}
	
	public SocketChannel getSocketChannel(){
		return socketChannel;
	}
	
	public void setSelectionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	public ExecutorService getExecutorService(){
		return this.executor;
	}
	
	public void doHandler(SelectionKey key) throws IOException {
		if(key.isValid()&&key.isWritable()){
			log.debug("socekt writable,write wait queue data");
			onWrite(key);
		}
		
		if(key.isValid()&&key.isReadable()){
			onRead(key);
		}
	}
	
	private void onWrite(SelectionKey key) throws IOException{
		if(hasWaitWriteQueue){
			if(waitQueue.isEmpty()){
				throw new IOException("writable when wait queue is empty");
			}
			
			while(!waitQueue.isEmpty()){
				ByteBuffer data = waitQueue.peek();
				if(write(data)){
					waitQueue.remove();
				}else{
					return;//block,wait next writable
				}
			}
			
			key.interestOps(SelectionKey.OP_READ);
			hasWaitWriteQueue = false;
			
		}else{
			throw new IOException("writable when wait queue is true");
		}
	}
	/**
	 * 
	 * @param data
	 * @return already write complete
	 */
	private synchronized boolean write(ByteBuffer data){
		try {
			while(data.hasRemaining()){
				int n = socketChannel.write(data);
				if(n==-1){
					selectionKey.cancel();
					throw new IOException("write exception");
				}
				if(n==0){
					return false;//block
				}
				n = socketChannel.write(data);
			}
			return true;
		} catch (IOException e) {
			e.printStackTrace();
			selectionKey.cancel();
			try {
				socketChannel.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
		return false;
	}
	
	/**
	 * need confirm thread safe
	 * @param buffer
	 */
	public void send(Buffer buffer){
		ByteBuffer data = buffer.getByteBuffer();
		if(hasWaitWriteQueue){
			waitQueue.add(data);
			return;
		}
		
		if(!write(data)){
			waitQueue.add(data);
			log.debug("session socket block fix selectionkey interest");
			hasWaitWriteQueue = true;
			selectionKey.interestOps(SelectionKey.OP_READ|SelectionKey.OP_WRITE);
			selectionKey.selector().wakeup();
		}
	}
	
	public void onRead(SelectionKey key) throws IOException{
		int read = socketChannel.read(buffer);
		if(read  < 0 ) throw new IOException("Read Socket Exception");
		while(read > 0 ){
			buffer.flip();
			while(buffer.remaining()>0){
				Object message = protocolHandler.decode(buffer);
				if(message!=null){
					log.debug("protocol decode complete");
					if(enableWorkerThread){
						executor.execute(new WorkerItem(this,message,handlerChain));
					}else{
						handlerChain.handle(this, message);
					}
				}
			}
			buffer.clear();
			read = socketChannel.read(buffer);
		}
	}
	
	private class WorkerItem implements Runnable{
		private SessionHandler session;
		private Object message;
		private MessageHandler chain;
		public WorkerItem(SessionHandler session,Object message,MessageHandler chain){
			this.session = session;
			this.message = message;
			this.chain = chain;
		}
		
		public void run() {
			chain.handle(session, message);
		}
	}
}
