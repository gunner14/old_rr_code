package com.renren.tripod.client;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.tripod.internalclient.CacheClient;
import com.renren.tripod.internalclient.CacheManagerClient;
import com.renren.tripod.internalclient.ClientFactory;
import com.renren.tripod.internalclient.RedisCacheClient;
import com.xiaonei.xce.log.Tripod;

public class TripodClient implements ITripodClient {

    private String _zkAddress = null;
    private String _namespaceId = null;
    private String _businessId = null;

    private CacheClient _cacheClient = null;
    private CacheManagerClient _cacheManagerClient = null;
    private RedisCacheClient redisCacheClient = null;

    public TripodClient(String zkAddress, String namespaceId, String businessId) {
	_zkAddress = zkAddress;
	_namespaceId = namespaceId;
	_businessId = businessId;
    }

    public Map<String, byte[]> get(List<String> keys, List<String> missedKeys) {
	CacheClient cc = getCacheClient();
	if (cc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.get cacheclient == null for zkAddress "
			    + _zkAddress);
	    return null;
	}
	Map<String, byte[]> res = cc.get(keys, _namespaceId, _businessId);
	getMissedKeys(res, keys, missedKeys);
	return res;
    }

    public Map<String, byte[]> getMissed(List<String> keys, int expiredTime,
	    boolean useMaster) {
	return getMissed(keys, expiredTime, useMaster, 300);
    }

    public Map<String, byte[]> getMissed(List<String> keys, int expiredTime,
	    boolean useMaster, int timeout) {
	CacheManagerClient cc = getCacheManagerClient();
	if (cc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.getMissed cachemanagerclient == null for zkAddress "
			    + _zkAddress);
	    return null;
	}
	return cc.get(keys, _namespaceId, _businessId, expiredTime, useMaster,
		timeout);
    }

    public Map<String, byte[]> getWithMissed(List<String> keys,
	    int expiredTime, boolean useMaster, int timeout) {
	List<String> missedKeys = new ArrayList<String>();
	Map<String, byte[]> res = get(keys, missedKeys);

	if (!missedKeys.isEmpty()) {
	    Map<String, byte[]> missedRes = getMissed(missedKeys, expiredTime,
		    useMaster, timeout);
	    res.putAll(missedRes);
	}
	return res;
    }

    public boolean load(List<String> keys, int expiredTime, boolean useMaster) {
	return load(keys, expiredTime, useMaster, 1000);
    }

    public boolean load(List<String> keys, int expiredTime, boolean useMaster,
	    int timeout) {
	CacheManagerClient cc = getCacheManagerClient();
	if (cc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.load cachemanagerclient == null for zkAddress "
			    + _zkAddress);
	    return false;
	}
	return cc.load(keys, _namespaceId, _businessId, expiredTime, useMaster,
		timeout);
    }

    public boolean deleteCache(List<String> keys) {
	return deleteCache(keys, 1000);
    }

    public boolean deleteCache(List<String> keys, int timeout) {
	CacheManagerClient cc = getCacheManagerClient();
	if (cc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.deleteCache cachemanagerclient == null for zkAddress "
			    + _zkAddress);
	    return false;
	}
	return cc.deleteCache(keys, _namespaceId, _businessId, timeout);
    }

    private List<String> getMissedKeys(Map<String, byte[]> res,
	    List<String> keys, List<String> missedKeys) {
	for (String str : keys) {
	    if (res.get(str) == null) {
		missedKeys.add(str);
	    }
	}
	return missedKeys;
    }

    private CacheClient getCacheClient() {
	if (_cacheClient == null) {
	    synchronized (this) {
		if (_cacheClient == null) {
		    _cacheClient = ClientFactory.getCachegClient(_zkAddress);
		}
	    }
	}
	return _cacheClient;
    }

    private CacheManagerClient getCacheManagerClient() {
	if (_cacheManagerClient == null) {
	    synchronized (this) {
		if (_cacheManagerClient == null) {
		    _cacheManagerClient = ClientFactory
			    .getCacheManagerClient(_zkAddress);
		}
	    }
	}
	return _cacheManagerClient;
    }

    private RedisCacheClient getRedisCacheClient() {
	if (redisCacheClient == null) {
	    synchronized (this) {
		if (redisCacheClient == null) {
		    redisCacheClient = ClientFactory
			    .getRedisCachegClient(_zkAddress);
		}
	    }
	}
	return redisCacheClient;
    }

    public Map<String, String> getHash(String key, List<String> fields) {
	Map<String, String> result = new HashMap<String, String>();
	RedisCacheClient rcc = getRedisCacheClient();
	if (rcc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.getHash getRedisCacheClient() == null for zkAddress "
			    + _zkAddress);
	    System.err
		    .println("TripodClient.getHash getRedisCacheClient() == null");
	    return result;
	}
	boolean exist = rcc.exists(key, _namespaceId, _businessId);
	if (!exist) {
	    // timeout for 10 secs
	    return getMissedHash(key, fields, true, 10000);
	} else {
	    result = rcc.getHash(key, fields, _namespaceId, _businessId);
	}
	return result;
    }

    public Map<String, String> getMissedHash(String key, List<String> fields,
	    boolean useMaster, int timeout) {
	Map<String, String> result = new HashMap<String, String>();
	CacheManagerClient cmc = getCacheManagerClient();
	if (cmc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.getMissedHash getCacheManagerClient() == null for zkAddress "
			    + _zkAddress);
	    System.err
		    .println("TripodClient.getMissedHash getCacheManagerClient() == null");
	}
	result = cmc.getHashCache(key, fields, _namespaceId, _businessId,
		useMaster, timeout);
	return result;
    }

    public boolean loadHash(String key, boolean useMaster, int timeout) {
	CacheManagerClient cmc = getCacheManagerClient();
	if (cmc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.loadHashCache getCacheManagerClient() == null for zkAddress "
			    + _zkAddress);
	    System.err
		    .println("TripodClient.loadHashCache getCacheManagerClient() == null");
	}
	return cmc.loadHashCache(key, _namespaceId, _businessId, useMaster,
		timeout);
    }

    public boolean exists(String key) {
	RedisCacheClient rcc = getRedisCacheClient();
	if (rcc == null) {
	    Tripod.getLogger().error(
		    "TripodClient.getHash getRedisCacheClient() == null for zkAddress "
			    + _zkAddress);
	    System.err
		    .println("TripodClient.getHash getRedisCacheClient() == null");
	    return false;
	}
	boolean exist = rcc.exists(key, _namespaceId, _businessId);
	return exist;
    }
}
