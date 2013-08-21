package com.xiaonei.login.cache;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.tripod.TripodException;
import xce.tripod.client.TripodCacheClient;

/**
 * 
 * @author wen.he1
 *
 */
public abstract class AbstractCommonCache {
		
	private static final Log logger = LogFactory.getLog(AbstractCommonCache.class); 
 	
	abstract public TripodCacheClient getClient(); 
 

	/**
	 * 取值  
	 * @param <T> key 
	 * @return 
	 */
	@SuppressWarnings("unchecked")
	public <T> T get(String key) {
		if (logger.isInfoEnabled()) {
			logger.info("AbstractCommonCache.get:key=" + key);   
		}
		 
		T obj = null;
		try {
			obj = (T)getClient().get(key);
			 
			if (logger.isInfoEnabled()) {
				logger.info("AbstractCommonCache.get:key=" + key + ",result=" + obj);
			}
		} catch (TripodException e) {
			logger.error("AbstractCommonCache.get:key=" + key , e);
		}
		return obj;
	}
	 
	
	/**
	 * 设置某个key
	 * 
	 * @param key
	 * @param obj
	 * @param expireTime 过期时间 
	 */
	public void set(String key, Object obj, int expireTime) {
		if (logger.isInfoEnabled()) {
			logger.info("AbstractCommonCache.set:key=" + key + ",obj=" + obj+",expireTime="+expireTime);
		}
		try {
			getClient().set(key, obj, expireTime);
		} catch (TripodException e) {
			logger.error("AbstractCommonCache.set:key=" + key + ",obj=" + obj+",expireTime="+expireTime , e);
		}
	}

	
	/**
	 * 删除某个key
	 * @param key
	 */
	public void remove(String key) {
		if (logger.isInfoEnabled()) {
			logger.info("AbstractCommonCache.remove:key=" + key);
		}
		try {
			getClient().remove(key);
		} catch (TripodException e) {
			logger.error("AbstractCommonCache.remove:key=" + key, e);
		} 
	}

	
}

