package com.renren.xcache.impl.memcachedclient;

import java.util.Map;

import com.renren.xcache.StatsManager;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-10-26 下午05:24:21
 */
public class StatsManagerImpl implements StatsManager {
	
	private XCacheImpl<?> xcache;
	
	public StatsManagerImpl (XCacheImpl<?> xcache) {
		this.xcache = xcache;
	}
	
	@Override
	public Map<String, Map<String, String>> stats() {
		return xcache.getClient().stats();
	}

	@Override
	public Map<String, Map<String, String>> statsItems() {
		return xcache.getClient().statsItems();
	}

	@Override
	public Map<String, Map<String, String>> statsCacheDump(int slabNumber, int limit) {
		return xcache.getClient().statsCacheDump(slabNumber, limit);
	}

	@Override
	public Map<String, Map<String, String>> statsSlabs() {
		return xcache.getClient().statsSlabs();
	}
	
}
