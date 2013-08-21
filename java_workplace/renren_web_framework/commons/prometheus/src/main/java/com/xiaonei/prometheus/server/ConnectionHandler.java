package com.xiaonei.prometheus.server;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;

import com.xiaonei.prometheus.LocalException;
import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.nio.IConnectionHandler;
import com.xiaonei.prometheus.nio.IProtocolHandler;
import com.xiaonei.prometheus.nio.SessionHandler;

public class ConnectionHandler implements IConnectionHandler{
	private volatile boolean isConnected = false;
	private CountDownLatch connectLatch = new CountDownLatch(1);
	private SessionHandler session;
	private IProtocolHandler protocol;
	private ConcurrentHashMap<Integer, ResultCallback<Message>> callbackMap = new ConcurrentHashMap<Integer, ResultCallback<Message>>();
	private Logger log = Logger.getLogger(PrometheusServer.class);
	
	public void setSessionHandler(SessionHandler session) {
		this.session = session;
	}
	
	public void completeConnect(){
		isConnected = true;
		connectLatch.countDown();
	}
	
	public void setProtocolHandler(IProtocolHandler protocol){
		this.protocol = protocol;
	}
	
	public void handle(SessionHandler session, Object receiver) {
		log.debug("connection handle message");
		if(this.session != session){
			throw new LocalException("Connection Session Exception");
		}
		
		Message message = (Message)receiver;
		ResultCallback<Message> callback = callbackMap.get(message.getSeqId());
		if(callback==null){
			throw new LocalException("message seq id is unavliad");
		}
		callback.call(message);
	}

	public ConnectionHandler(){
		
	}
	
	
	/**
	 * block unit return result 
	 * @param buffer
	 * @return
	 */
	public Message syncWrite(Message message){
		if(!isConnected){
			try {
				connectLatch.await();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		Buffer buffer = protocol.encode(message);
		int seqId = message.getSeqId();
		ResultCallback<Message> callback = new ResultCallback<Message>();
		callbackMap.put(seqId, callback);
		session.send(buffer);
		return callback.get();
	}
	
	public static class ResultCallback<T>{
		private CountDownLatch latch = new CountDownLatch(1);
		private T result;
		//TODO:add timeout
		public void call(T message){
			result = message;
			latch.countDown();
		}
		public T get(){
			try {
				latch.await();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			return result;
		}
	}
}
