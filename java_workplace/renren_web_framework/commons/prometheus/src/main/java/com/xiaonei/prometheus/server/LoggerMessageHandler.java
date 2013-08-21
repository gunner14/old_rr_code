package com.xiaonei.prometheus.server;

import com.xiaonei.prometheus.nio.MessageHandler;
import com.xiaonei.prometheus.nio.SessionHandler;

public class LoggerMessageHandler implements MessageHandler{
	public void handle(SessionHandler session,Object msg) {
		Message message = (Message)msg;
		System.out.println("new message receiver requestId = "+message.getSeqId());
	}

}
