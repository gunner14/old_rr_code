package com.xiaonei.jade.cache;


import net.paoding.rose.jade.statement.cached.CacheInterface;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * 提供 memcache 缓存的 {@link CacheInterface} 实现。
 * 
 * @author han.liao
 */
public class MemCache implements CacheInterface {

    private static Log logger = LogFactory.getLog(MemCache.class);

    private final MemCacheManager memCacheManager;

    private String poolName; // 缓存名称

    public MemCache(String poolName, MemCacheManager memCacheManager) {
        this.memCacheManager = memCacheManager;
        this.poolName = poolName;
    }

    @Override
    public Object get(String key) {

        Object value = memCacheManager.get(key);

        if (logger.isDebugEnabled()) {
            logger.debug("Get cache \'" + key + "\' from pool \'" + poolName + "\': " + value);
        }

        return value;
    }

    @Override
    public boolean set(String key, Object value, int expiry) {

        boolean cached = memCacheManager.set(key, value, expiry);

        if (logger.isDebugEnabled()) {
            logger.debug("[" + Boolean.toString(cached) + "] Set cache \'" + key
                    + "\' from pool \'" + poolName + "\': " + value);
        }

        return cached;
    }

    @Override
    public boolean delete(String key) {

        boolean deleted = memCacheManager.delete(key);

        if (logger.isDebugEnabled()) {
            logger.debug("[" + Boolean.toString(deleted) + "] Delete cache \'" + key
                    + "\' from pool \'" + poolName + "\'.");
        }

        return deleted;
    }
}
