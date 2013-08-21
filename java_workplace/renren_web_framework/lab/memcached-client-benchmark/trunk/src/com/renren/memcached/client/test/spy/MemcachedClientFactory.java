package com.renren.memcached.client.test.spy;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.spy.memcached.KetamaConnectionFactory;
import net.spy.memcached.MemcachedClient;

import com.renren.memcached.client.test.Server;
import com.renren.memcached.client.test.ServerConfig;

public class MemcachedClientFactory {
	
	private static Map<Integer, MemcachedClient> clientPool = new HashMap<Integer, MemcachedClient>();
	
	public static MemcachedClient getClient(int nodes) {
		MemcachedClient client = clientPool.get(nodes);
		if (client == null) {
			initClient(nodes);
		}
		return clientPool.get(nodes);
	}
	
	
	
	private static synchronized void initClient(int nodes) {
		
		if (clientPool.get(nodes) == null) {
			// server list and weights
			//String[] allServers = ServerConfig.getInstance().getServersArray();
			//String[] servers = Arrays.copyOfRange(allServers, 0, nodes);

			List<Server> allServers = ServerConfig.getInstance().getServers();
			List<Server> servers = allServers.subList(0, nodes);
			
			List<InetSocketAddress> addrs = new ArrayList<InetSocketAddress>();
			for(Server s : servers) {
				addrs.add(new InetSocketAddress(s.getHost(), s.getPort()));
			}
			
			try {
				MemcachedClient client = new MemcachedClient(new KetamaConnectionFactory(), addrs);
				clientPool.put(nodes, client);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}



	public static MemcachedClient getClient() {
		return getClient(1);
	}
}
