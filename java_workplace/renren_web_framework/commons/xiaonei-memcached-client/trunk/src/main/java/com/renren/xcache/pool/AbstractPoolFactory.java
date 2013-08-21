package com.renren.xcache.pool;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * {@link CachePoolFactory}的基类实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-21 下午06:14:25
 */
public abstract class AbstractPoolFactory implements CachePoolFactory {

	protected Log logger = LogFactory.getLog(this.getClass());
	
	protected Map<String, CachePool> pools = new ConcurrentHashMap<String, CachePool>();
	
	protected List<PoolConfigModifiedCallback> callbacks = new ArrayList<PoolConfigModifiedCallback>();
	
	@Override
	public CachePool getPool(String poolName) {
		CachePool pool = pools.get(poolName);
		if (pool == null) {
			synchronized (poolName.intern()) {
				pool = pools.get(poolName);
				if (pool == null) {
					pool = loadPoolConfig(poolName);
				}
				if (pool != null) {
					pools.put(poolName, pool);
				}
			}
		}
		return pool;
	}

	protected abstract CachePool loadPoolConfig(String poolName);
	
	@Override
	public void addCallback(PoolConfigModifiedCallback callback) {
		callbacks.add(callback);
	}

	protected void onPoolModified(CachePool newPool) {
		String poolName = newPool.getName();
		CachePool oldPool = pools.get(poolName);
		if (oldPool != null) {
		
			if (newPool.equals(oldPool)) {
				if (logger.isInfoEnabled()) {
					logger.info("pool config not modified:" + oldPool);
				}
			} else {
				pools.put(poolName, newPool);
				if (logger.isInfoEnabled()) {
					logger.info("pool config modified, old=" + oldPool + "\r\nnew=" + newPool);
				}
				for (PoolConfigModifiedCallback callback : callbacks) {
					try {
						callback.onPoolModified(newPool);
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}
		} else {
			logger.error("Original pool not existed, this should never happen: " + poolName);
		}
	}
	
}
