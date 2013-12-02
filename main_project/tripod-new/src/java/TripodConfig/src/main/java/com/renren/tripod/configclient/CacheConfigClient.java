package com.renren.tripod.configclient;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import com.danga.MemCached.MemCachedClient;
import com.danga.MemCached.SockIOPool;

import com.renren.tripod.configutil.ConfigClient;
import com.renren.tripod.configutil.ConfigData;

public class CacheConfigClient extends ConfigClient {
	private static org.apache.commons.logging.Log _logger = org.apache.commons.logging.LogFactory
			.getLog(CacheConfigClient.class);

	private ConcurrentHashMap<String, HashHolder<MemCachedClient>> _holderMap = new ConcurrentHashMap<String, HashHolder<MemCachedClient>>();

	public CacheConfigClient(String zkAddress, String identifier) {
		super(zkAddress, identifier);
		_logger.info("CacheConfigClient.CacheConfigClient zkAddress "
				+ zkAddress + " identifier " + identifier);
	}

	@Override
	protected boolean update(ConfigData newConfig) {
		_logger.info("CacheConfigClient.update");
		ConfigData cacheConfig = newConfig._childrenDatas
				.get(ConfigData.TRIPOD_CACHE);
		if (cacheConfig == null) {
			_logger.error("CacheConfigClient.update cacheConfig == null!");
			return false;
		}

		for (Entry<String, ConfigData> nConfig : newConfig._childrenDatas
				.get(ConfigData.TRIPOD_CACHE)._childrenDatas.entrySet()) {
			HashHolder<MemCachedClient> tHH = new HashHolder<MemCachedClient>(
					100);
			Map<String, MemCachedClient> tClients = new HashMap<String, MemCachedClient>();
			for (Entry<String, ConfigData> cConfig : nConfig.getValue()._childrenDatas
					.entrySet()) {
				String[] parts = cConfig.getKey().split(":");
				String hashStr = "";
				String host = null;
				String port = null;
				if (parts.length == 2) {
					hashStr = cConfig.getKey();
					host = parts[0];
					port = parts[1];
				} else if (parts.length >= 3) {
					for (int i = 0; i < parts.length - 2; ++i) {
						hashStr += parts[i];
						if (i != parts.length - 3) {
							hashStr += ":";
						}
					}
					host = parts[parts.length - 2];
					port = parts[parts.length - 1];
				} else {
					return false;
				}
				SockIOPool pool = SockIOPool.getInstance(host + ":" + port);
				String[] address = new String[] { host + ":" + port };
				if (!pool.isInitialized()) {
					pool.setServers(address);
					pool.setInitConn(1);
					pool.setMinConn(1);
					pool.setMaxConn(50);
					pool.setMaxIdle(1000 * 60 * 60 * 6);
					pool.setNagle(false);
					pool.setSocketTO(300);
					pool.setMaintSleep(30);
					pool.setSocketConnectTO(0);
					pool.initialize();
				}

				MemCachedClient tMcc = new MemCachedClient(host + ":" + port);
				tClients.put(hashStr, tMcc);
			}
			tHH.renew(tClients);
			_holderMap.put(nConfig.getKey(), tHH);
		}

		return true;
	}

	public MemCachedClient getPrx(String key, String namespaceId) {
		HashHolder<MemCachedClient> hh = _holderMap.get(namespaceId);
		if (hh != null) {
			return hh.get(key);
		}
		_logger.error("CacheConfigClient.getPrx no prx for namespaceId "
				+ namespaceId);
		return null;
	}

	public Map<MemCachedClient, List<String>> getPrxToKeysMap(
			List<String> keys, String namespaceId) {
		HashHolder<MemCachedClient> hh = _holderMap.get(namespaceId);
		if (hh != null) {
			return hh.get(keys);
		}
		_logger.error("CacheConfigClient.getPrx no prx for namespaceId "
				+ namespaceId);
		return null;
	}

}
