package com.renren.tripod.internalclient;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import com.renren.tripod.configclient.ConfigClientFactory;
import com.renren.tripod.configclient.RedisCacheConfigClient;
import com.xiaonei.xce.log.Tripod;

/**
 * RedisCacheClient
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class RedisCacheClient {
    private RedisCacheConfigClient configClient = null;
    private String zkAddress = null;

    public RedisCacheClient(String zkAddress) {
	this.zkAddress = zkAddress;
    }

    private RedisCacheConfigClient getConfigClient() {
	if (configClient == null) {
	    synchronized (this) {
		if (configClient == null) {
		    configClient = ConfigClientFactory
			    .getRedisCacheConfigClient(zkAddress);
		}
	    }
	}
	return configClient;
    }

    /**
     * 取Hash Cache
     * 
     * @param key
     * @param fields
     * @param namespaceId
     * @param bizId
     * @return
     */
    public Map<String, String> getHash(String key, List<String> fields,
	    String namespaceId, String bizId) {
	Map<String, String> result = new HashMap<String, String>();
	if (fields == null || fields.isEmpty()) {
	    return result;
	}
	RedisCacheConfigClient configClient = getConfigClient();
	if (configClient == null) {
	    Tripod.getLogger().error(
		    "RedisCacheClient.getConfig == null, namespaceId "
			    + namespaceId + " bussinessId " + bizId);
	    System.err
		    .println("RedisCacheClient.getConfig == null, namespaceId "
			    + namespaceId + " bussinessId " + bizId);
	    return result;
	}
	JedisPool pool = configClient.getPrx(key, namespaceId);
	if (pool == null) {
	    System.err.println("pool == null!!! namespaceId:" + namespaceId
		    + "\tbizId:" + bizId + "\tkey:" + key);
	    return result;
	}
	Jedis jedis = null;
	try {
	    jedis = pool.getResource();
	    String cacheKey = getCacheKey(key, bizId);
	    String[] arrayString = new String[fields.size()];
	    for (int i = 0; i < fields.size(); ++i) {
		arrayString[i] = fields.get(i);
	    }
	    List<String> list = jedis.hmget(cacheKey, arrayString);
	    for (int i = 0; i < fields.size(); ++i) {
		result.put(fields.get(i), list.get(i));
	    }
	    pool.returnResource(jedis);
	} catch (Exception e) {
	    e.printStackTrace();
	    if (jedis != null) {
		pool.returnBrokenResource(jedis);
	    }
	}
	return result;
    }

    /**
     * 判断是否存在
     * 
     * @param key
     * @param namespaceId
     * @param bizId
     * @return
     */
    public boolean exists(String key, String namespaceId, String bizId) {
	boolean result = false;
	RedisCacheConfigClient configClient = getConfigClient();
	if (configClient == null) {
	    Tripod.getLogger().error(
		    "RedisCacheClient.getConfig == null, namespaceId "
			    + namespaceId + " bussinessId " + bizId);
	    System.err
		    .println("RedisCacheClient.getConfig == null, namespaceId "
			    + namespaceId + " bussinessId " + bizId);
	    return false;
	}
	JedisPool pool = configClient.getPrx(key, namespaceId);
	if (pool == null) {
	    System.err.println("pool == null!!! namespaceId:" + namespaceId
		    + "\tbizId:" + bizId + "\tkey:" + key);
	    return false;
	}
	Jedis jedis = null;
	try {
	    jedis = pool.getResource();
	    String cacheKey = getCacheKey(key, bizId);
	    result = jedis.exists(cacheKey);
	    pool.returnResource(jedis);
	} catch (Exception e) {
	    e.printStackTrace();
	    if (jedis != null) {
		pool.returnBrokenResource(jedis);
	    }
	}
	return result;
    }

    private static String SEPARATOR = "_";

    private static String getCacheKey(String rawKey, String biz_id) {
	return biz_id + SEPARATOR + rawKey;
    }

    private static String getRawKey(String cacheKey, String biz_id) {
	return cacheKey.substring(biz_id.length() + 1, cacheKey.length());
    }
}
