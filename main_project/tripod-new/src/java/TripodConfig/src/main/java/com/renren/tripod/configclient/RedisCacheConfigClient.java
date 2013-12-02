package com.renren.tripod.configclient;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import redis.clients.jedis.JedisPool;

import com.renren.tripod.configutil.ConfigClient;
import com.renren.tripod.configutil.ConfigData;

/**
 * RedisCacheConfigClient
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class RedisCacheConfigClient extends ConfigClient {
    private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
	    .getLog(CacheConfigClient.class);

    private ConcurrentHashMap<String, HashHolder<JedisPool>> namespaceToHolderMap = new ConcurrentHashMap<String, HashHolder<JedisPool>>();

    public RedisCacheConfigClient(String zkAddress, String identifier) {
	super(zkAddress, identifier);
    }

    @Override
    protected boolean update(ConfigData newConfig) {
	ConfigData cacheConfig = newConfig._childrenDatas
		.get(ConfigData.TRIPOD_CACHE);
	if (cacheConfig == null) {
	    logger.error("CacheConfigClient.update cacheConfig == null!");
	    return false;
	}
	for (Entry<String, ConfigData> namespaceConfig : cacheConfig._childrenDatas
		.entrySet()) {
	    String namespace = namespaceConfig.getKey();
	    Map<String, JedisPool> poolMap = new HashMap<String, JedisPool>();
	    for (Entry<String, ConfigData> nodeConfig : namespaceConfig
		    .getValue()._childrenDatas.entrySet()) {
		String configString = nodeConfig.getKey();
		// new config
		String[] parts = configString.split(":");
		String hashString = "";
		String host = null;
		int port;
		if (parts.length == 2) {
		    hashString = configString;
		    host = parts[0];
		    port = Integer.parseInt(parts[1]);
		} else if (parts.length >= 3) {
		    for (int i = 0; i < parts.length - 2; ++i) {
			hashString += parts[i];
			if (i != parts.length - 3) {
			    hashString += ":";
			}
		    }
		    host = parts[parts.length - 2];
		    port = Integer.parseInt(parts[parts.length - 1]);
		} else {
		    return false;
		}
		JedisPool pool = new JedisPool(host, port);
		poolMap.put(hashString, pool);
	    }
	    if (!poolMap.isEmpty()) {
		HashHolder<JedisPool> holder = new HashHolder<JedisPool>(100);
		holder.renew(poolMap);
		namespaceToHolderMap.put(namespace, holder);
	    }
	}
	return true;
    }

    public JedisPool getPrx(String key, String namespace) {
	HashHolder<JedisPool> holder = namespaceToHolderMap.get(namespace);
	if (holder != null) {
	    return holder.get(key);
	}
	return null;
    }
    
    
    public static void main(String[] args) {
	
    }

}
