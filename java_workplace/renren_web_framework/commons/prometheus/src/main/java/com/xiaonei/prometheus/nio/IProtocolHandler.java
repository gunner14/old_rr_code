package com.xiaonei.prometheus.nio;

import java.nio.ByteBuffer;

public interface IProtocolHandler {
	public Buffer encode(Object message);
	public Object decode(ByteBuffer buffer);
}
