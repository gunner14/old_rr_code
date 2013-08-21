package com.xiaonei.prometheus.nio;

import java.io.IOException;

public interface EventHandler {
	public void doHandler(java.nio.channels.SelectionKey key) throws IOException;
}
