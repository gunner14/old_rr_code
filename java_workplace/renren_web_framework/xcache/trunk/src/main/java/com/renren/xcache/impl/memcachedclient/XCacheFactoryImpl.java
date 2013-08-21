package com.renren.xcache.impl.memcachedclient;

import com.danga.MemCached.MemCachedClient;
import com.renren.xcache.XCache;
import com.renren.xcache.XCacheFactory;

/**
 * 
 *
 */
public class XCacheFactoryImpl implements XCacheFactory {

    private MemcachedClientFactory memcachedClientFactory = new MemcachedClientFactory();

    public MemCachedClient getMemcachedClient(String namespace) {
        return memcachedClientFactory.getClientByNamespace(namespace);
    }

	@Override
	public <T> XCache<T> getCache(String prefix, Class<T> valueClass, boolean isCounter) {
		return new XCacheImpl<T>(prefix, this, valueClass, isCounter);
	}
}
