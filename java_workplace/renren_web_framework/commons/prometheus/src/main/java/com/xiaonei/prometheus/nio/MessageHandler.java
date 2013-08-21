package com.xiaonei.prometheus.nio;

public interface MessageHandler{
	public void handle(SessionHandler session,Object message);
}
