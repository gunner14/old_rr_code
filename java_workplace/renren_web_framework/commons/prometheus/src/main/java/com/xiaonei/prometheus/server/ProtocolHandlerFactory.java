package com.xiaonei.prometheus.server;

import com.xiaonei.prometheus.nio.IProtocolHandler;
import com.xiaonei.prometheus.nio.IProtocolHandlerFactory;

public class ProtocolHandlerFactory implements IProtocolHandlerFactory{
	public IProtocolHandler factory() {
		return new ProtocolHandler();
	}
}
