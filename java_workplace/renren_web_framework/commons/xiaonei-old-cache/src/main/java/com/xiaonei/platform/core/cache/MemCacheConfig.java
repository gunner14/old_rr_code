package com.xiaonei.platform.core.cache;
final class MemCacheConfig{
	public final String poolName;
	public final String hostName;
	public final int port;
	public final int connectionNum;
	
	public MemCacheConfig(String poolName,String hostName,int port,int connectionNum){
		this.poolName = poolName;
		this.hostName = hostName;
		this.port = port;
		this.connectionNum = connectionNum;
	}
	public String toString(){
		return poolName+":"+hostName+":"+port+":"+connectionNum;
	}
}