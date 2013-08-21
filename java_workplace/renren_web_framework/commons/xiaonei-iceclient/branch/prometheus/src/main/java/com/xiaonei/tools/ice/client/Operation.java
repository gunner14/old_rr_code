package com.xiaonei.tools.ice.client;

import java.nio.ByteBuffer;

public interface Operation {
	public int getRequestId();
	public void setRequestId(int requestId);
	public ByteBuffer getBuffer();
	public void writeComplete();
	public void callback(ByteBuffer data);
}
