package com.xiaonei.platform.core.cache;

import net.spy.memcached.KetamaConnectionFactory;

public class MemCacheConnectionFactory extends KetamaConnectionFactory{
	private int default_timeout = 2000;
	
	public MemCacheConnectionFactory(){
		super();
	}
	
	public long getOperationTimeout() {
		return default_timeout;
	}
}
