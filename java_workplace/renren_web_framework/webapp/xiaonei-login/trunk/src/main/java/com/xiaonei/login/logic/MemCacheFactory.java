package com.xiaonei.login.logic;

import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.annotation.TimeCost;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

@Singleton
@TimeCost
public class MemCacheFactory {

	private com.xiaonei.platform.core.cache.MemCacheFactory memCacheFactory = com.xiaonei.platform.core.cache.MemCacheFactory
			.getInstance();

	public MemCacheManager getManagerByPoolName(MemCacheKeys pool) {
		return memCacheFactory.getManagerByPoolName(pool);
	}

	public int hashCode() {
		return memCacheFactory.hashCode();
	}

	public void stopMonitor() {
		memCacheFactory.stopMonitor();
	}

	public String toString() {
		return memCacheFactory.toString();
	}

	private static final MemCacheFactory instance = new MemCacheFactory();

	public static MemCacheFactory getInstance() {
		return instance;
	}

}
