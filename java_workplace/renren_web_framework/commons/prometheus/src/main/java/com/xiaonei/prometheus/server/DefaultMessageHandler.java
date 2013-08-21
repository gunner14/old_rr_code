package com.xiaonei.prometheus.server;

import com.xiaonei.prometheus.LocalException;
import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.nio.MessageHandler;
import com.xiaonei.prometheus.nio.SessionHandler;

public class DefaultMessageHandler implements MessageHandler {
	public void handle(SessionHandler session, Object msg) {
		Message message = (Message)msg;
		byte opcode = message.getOpcode();
		if(opcode==Protocol.OPCODE_GET){
			handleGet(session,message);
		}else{
			throw new LocalException("error message");
		}
	}
	
	//private static Store store = new CabinetStore("/home/wenwen/workspace/Prometheus/bin/test.tdb");

	private void handleGet(SessionHandler session, Message message) {
		Buffer buffer = message.getData();
		buffer = session.getProtocolHandler().encode(message);
		session.send(buffer);
	}
}
