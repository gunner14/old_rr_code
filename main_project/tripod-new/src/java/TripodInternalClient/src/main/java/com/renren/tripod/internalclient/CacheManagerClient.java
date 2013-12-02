package com.renren.tripod.internalclient;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import com.renren.tripod.CacheManagerPrx;
import com.renren.tripod.CacheManagerPrxHelper;
import com.renren.tripod.configclient.CacheManagerConfigClient;
import com.renren.tripod.configclient.ConfigClientFactory;
import com.xiaonei.xce.log.Tripod;

public class CacheManagerClient {

    private CacheManagerConfigClient _config = null;
    private String _zkAddress = null;

    public CacheManagerClient(String zkAddress) {
	_zkAddress = zkAddress;
    }

    private CacheManagerConfigClient getConfigClient() {
	if (_config == null) {
	    synchronized (this) {
		_config = ConfigClientFactory
			.getCacheMangerConfigClient(_zkAddress);
	    }
	}
	return _config;
    }

    public Map<String, byte[]> get(List<String> keys, String namespaceId,
	    String businessId, int expiredTime, boolean useMaster) {
	return get(keys, namespaceId, businessId, expiredTime, useMaster, 300);
    }

    public Map<String, byte[]> get(List<String> keys, String namespaceId,
	    String businessId, int expiredTime, boolean useMaster, int timeout) {
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.get config == null, namespaceId "
			    + namespaceId + " businessId" + businessId);
	    return null;
	}
	Map<CacheManagerPrx, List<String>> prxMap = config
		.getPrxToKeysMap(keys);
	Map<String, byte[]> res = new HashMap<String, byte[]>();
	for (Entry<CacheManagerPrx, List<String>> ent : prxMap.entrySet()) {
	    CacheManagerPrx prx = CacheManagerPrxHelper.uncheckedCast(ent
		    .getKey().ice_twoway().ice_timeout(timeout));
	    Map<String, byte[]> singleRes = prx.get(
		    keys.toArray(new String[keys.size()]), namespaceId,
		    businessId, expiredTime, useMaster);
	    res.putAll(singleRes);
	}
	return res;
    }

    public boolean load(List<String> keys, String namespaceId,
	    String businessId, int expiredTime, boolean useMaster) {
	return load(keys, namespaceId, businessId, expiredTime, useMaster, 1000);
    }

    public boolean load(List<String> keys, String namespaceId,
	    String businessId, int expiredTime, boolean useMaster, int timeout) {
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.load config == null, namespaceId "
			    + namespaceId + " businessId " + businessId);
	    return false;
	}
	Map<CacheManagerPrx, List<String>> prxMap = config
		.getPrxToKeysMap(keys);
	boolean sucFlag = true;
	for (Entry<CacheManagerPrx, List<String>> ent : prxMap.entrySet()) {
	    CacheManagerPrx prx = CacheManagerPrxHelper.uncheckedCast(ent
		    .getKey().ice_twoway().ice_timeout(timeout));
	    boolean flag = prx.load(
		    ent.getValue().toArray(new String[ent.getValue().size()]),
		    namespaceId, businessId, expiredTime, useMaster);
	    sucFlag &= flag;
	}
	return sucFlag;
    }

    public boolean deleteCache(List<String> keys, String namespaceId,
	    String businessId) {
	return deleteCache(keys, namespaceId, businessId, 1000);
    }

    public boolean deleteCache(List<String> keys, String namespaceId,
	    String businessId, int timeout) {
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.deleteCache config == null, namespaceId "
			    + namespaceId + " businessId " + businessId);
	    return false;
	}
	Map<CacheManagerPrx, List<String>> prxMap = config
		.getPrxToKeysMap(keys);
	boolean sucFlag = true;
	for (Entry<CacheManagerPrx, List<String>> ent : prxMap.entrySet()) {
	    CacheManagerPrx prx = CacheManagerPrxHelper.uncheckedCast(ent
		    .getKey().ice_twoway().ice_timeout(timeout));
	    boolean flag = prx.deleteCache(
		    keys.toArray(new String[keys.size()]), namespaceId,
		    businessId);
	    sucFlag &= flag;
	}
	return sucFlag;
    }

    public boolean loadHashCache(String key, String namespaceId, String bizId,
	    boolean useMaster, int timeout) {
	boolean result = false;
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.loadHashCache config == null, namespaceId "
			    + namespaceId + " businessId " + bizId);
	    return false;
	}
	List<String> keys = new ArrayList<String>();
	keys.add(key);
	Map<CacheManagerPrx, List<String>> proxyMap = config
		.getPrxToKeysMap(keys);
	for (Entry<CacheManagerPrx, List<String>> entry : proxyMap.entrySet()) {
	    try {
		CacheManagerPrx proxy = CacheManagerPrxHelper
			.uncheckedCast(entry.getKey().ice_twoway()
				.ice_timeout(timeout));
		for (String k : entry.getValue()) { // just one loop
		    result = proxy.loadHashCache(k, namespaceId, bizId,
			    useMaster);
		}
	    } catch (Exception e) {
		e.printStackTrace();
		result = false;
		break;
	    }

	}
	return result;
    }

    public boolean deleteListOrHashCache(String key, String namespaceId,
	    String bizId, int timeout) {
	boolean result = false;
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.deleteListOrHashCache config == null, namespaceId "
			    + namespaceId + " businessId " + bizId);
	    return false;
	}
	List<String> keys = new ArrayList<String>();
	keys.add(key);
	Map<CacheManagerPrx, List<String>> proxyMap = config
		.getPrxToKeysMap(keys);
	for (Entry<CacheManagerPrx, List<String>> entry : proxyMap.entrySet()) {
	    try {
		CacheManagerPrx proxy = CacheManagerPrxHelper
			.uncheckedCast(entry.getKey().ice_twoway()
				.ice_timeout(timeout));
		for (String k : entry.getValue()) { // just one loop
		    result = proxy.deleteListOrHashCache(k, namespaceId, bizId);
		}
	    } catch (Exception e) {
		e.printStackTrace();
		result = false;
		break;
	    }

	}
	return result;
    }

    public Map<String, String> getHashCache(String key, List<String> fields,
	    String namespaceId, String bizId, boolean useMaster, int timeout) {
	Map<String, String> result = new HashMap<String, String>();
	CacheManagerConfigClient config = getConfigClient();
	if (config == null) {
	    Tripod.getLogger().error(
		    "CacheManagerClient.getHashCache config == null, namespaceId "
			    + namespaceId + " businessId " + bizId);
	    return result;
	}
	List<String> keys = new ArrayList<String>();
	keys.add(key);
	Map<CacheManagerPrx, List<String>> proxyMap = config
		.getPrxToKeysMap(keys);
	for (Entry<CacheManagerPrx, List<String>> entry : proxyMap.entrySet()) {
	    try {
		CacheManagerPrx proxy = CacheManagerPrxHelper
			.uncheckedCast(entry.getKey().ice_twoway()
				.ice_timeout(timeout));
		for (String k : entry.getValue()) { // just one loop
		    String[] fieldArray = new String[fields.size()];
		    for (int i = 0; i < fields.size(); ++i) {
			fieldArray[i] = fields.get(i);
		    }
		    result = proxy.getHashCache(key, fieldArray, namespaceId,
			    bizId, useMaster);
		}
	    } catch (Exception e) {
		e.printStackTrace();
		break;
	    }
	}
	return result;
    }
}
