package com.xiaonei.prometheus.nio;

import java.util.LinkedList;

public class MessageHandlerChain implements MessageHandler{
	
	public void handle(SessionHandler session,Object message){
		for(MessageHandler handler:handlerChain){
			handler.handle(session, message);
		}
	}
	
	public void addHandler(MessageHandler handler){
		handlerChain.add(handler);
	}
	
	private LinkedList<MessageHandler> handlerChain = new LinkedList<MessageHandler>();
	
}
