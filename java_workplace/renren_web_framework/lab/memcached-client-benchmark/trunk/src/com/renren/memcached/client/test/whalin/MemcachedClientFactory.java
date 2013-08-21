package com.renren.memcached.client.test.whalin;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import com.danga.MemCached.MemCachedClient;
import com.danga.MemCached.SockIOPool;
import com.renren.memcached.client.test.ServerConfig;

public class MemcachedClientFactory {

	

	// set up connection pool once at class load
	static {

		
	}
	
	private static Map<Integer, MemCachedClient> clientPool = new HashMap<Integer, MemCachedClient>();
	
	public static MemCachedClient getClient(int nodes) {
		MemCachedClient client = clientPool.get(nodes);
		if (client == null) {
			initClient(nodes);
		}
		return clientPool.get(nodes);
	}
	
	
	
	private static synchronized void initClient(int nodes) {
		
		if (clientPool.get(nodes) == null) {
			// server list and weights
			String[] allServers = ServerConfig.getInstance().getServersArray();
			String[] servers = Arrays.copyOfRange(allServers, 0, nodes);

			//Integer[] weights = { 3, 3, 2 };

			// grab an instance of our connection pool
			SockIOPool pool = SockIOPool.getInstance();

			// set the servers and the weights
			pool.setServers(servers);
			//pool.setWeights(weights);

			// set some basic pool settings
			// 5 initial, 5 min, and 250 max conns
			// and set the max idle time for a conn
			// to 6 hours
			pool.setInitConn(5);
			pool.setMinConn(5);
			pool.setMaxConn(250);
			pool.setMaxIdle(1000 * 60 * 60 * 6);

			// set the sleep for the maint thread
			// it will wake up every x seconds and
			// maintain the pool size
			pool.setMaintSleep(30);

			// set some TCP settings
			// disable nagle
			// set the read timeout to 3 secs
			// and don’t set a connect timeout
			pool.setNagle(false);
			pool.setSocketTO(3000);
			pool.setSocketConnectTO(0);

			// initialize the connection pool
			pool.initialize();
			
			// lets set some compression on for the client
			// compress anything larger than 64k
			//mcc.setCompressEnable(true);
			//mcc.setCompressThreshold(64 * 1024);
			
			
			clientPool.put(nodes, new MemCachedClient());
		}
	}



	public static MemCachedClient getClient() {
		return getClient(1);
	}
}
