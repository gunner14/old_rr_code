package com.xiaonei.prometheus.nio;

public interface IConnectionHandler extends MessageHandler {
	public void setSessionHandler(SessionHandler session);
	public void setProtocolHandler(IProtocolHandler protocol);
	public void completeConnect();
}
