package com.renren.xoa2.quota.cache;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

/**
 *
 * @author Xue Wenxin Nov 10, 2012 5:25:56 PM
 */
public class QuotaCacheImpl implements IQuotaCache {
	private Log logger = LogFactory.getLog(QuotaCacheImpl.class);
	
	/**获取通用cache的计数器实例*/
	public static TripodCacheClient counterClient = TripodCacheClientFactory.getClient(EntryType.BIZ_XOA_QUOTA); 
	
	/**用于非counter内容的通用cache实例*/
	public static TripodCacheClient tripodCacheClient = TripodCacheClientFactory.getClient(EntryType.BIZ_XOA_QUOTA_CACHE);
	private static int ERROR_CODE = 0;

	public boolean remove(String key) {
		try {
			counterClient.remove(key);
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	public void setLong(String key, long value, int expireTime) throws QuotaCacheException {
		if(logger.isInfoEnabled()){
			logger.info("setLong:key="+key+",value="+value+",expireTime="+expireTime);
		}
		try {
			counterClient.setLong(key, value, expireTime);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}		
	}

	public long getLong(String key) throws QuotaCacheException {
		if(logger.isInfoEnabled()){
			logger.info("getLong:key="+key);
		}
		try {
			return counterClient.getLong(key);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}
	}

	public long incLong(String key, long value) throws QuotaCacheException {
		long result = 0;
		if(logger.isInfoEnabled()){
			logger.info("incLong:key="+key+",value="+value);
		}
		try {
			result = counterClient.incLong(key, value);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}
		return result;
	}
	
	@SuppressWarnings("unchecked")
	public Set<String> getIps(String key) {
		if(logger.isInfoEnabled()){
			logger.info("getIps:key="+key);
		}
		try {
			return (Set<String>) tripodCacheClient.get(key);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public void putIp(String key, String ip, int expireTime) throws QuotaCacheException {
		if(logger.isInfoEnabled()){
			logger.info("putIps:key="+key+",ip="+ip+",expireTime="+expireTime);
		}
		try {
			@SuppressWarnings("unchecked")
			Set<String> ips = (Set<String>)tripodCacheClient.get(key);
			if(ips == null){
				ips = new HashSet<String>();
			}
			ips.add(ip);
			tripodCacheClient.set(key, ips, expireTime);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}	
	}

	public long getSingleQuotaUsed(String level,String service,String version, String clientId, String ip) throws QuotaCacheException {
		String key = level+"_"+service+"_"+version+"_"+clientId+"_"+ip;
		if(logger.isInfoEnabled()){
			logger.info("getSingleQuotaUsed:key="+key);
		}
		try {
			return counterClient.getLong(key);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}
	}
	
	public long getTotalQuotaUsed(String level,String service,String version, String clientId) throws QuotaCacheException {
		String key = level+"_"+service+"_"+version+"_"+clientId;
		if(logger.isInfoEnabled()){
			logger.info("getTotalQuotaUsed:key="+key);
		}
		try {
			return counterClient.getLong(key);
		} catch (Exception e) {
			logger.error("Error while operating on key-prefix: " + key);
			throw new QuotaCacheException(ERROR_CODE,"Error while operating on key-prefix: " + key);
		}
	}
}
