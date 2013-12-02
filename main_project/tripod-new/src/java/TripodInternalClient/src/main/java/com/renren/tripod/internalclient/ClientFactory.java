package com.renren.tripod.internalclient;

import java.util.concurrent.ConcurrentHashMap;

public class ClientFactory {
    private static org.apache.commons.logging.Log _logger = org.apache.commons.logging.LogFactory
	    .getLog(ClientFactory.class);
    private static ConcurrentHashMap<String, CacheManagerClient> _cacheManagerClientMap = new ConcurrentHashMap<String, CacheManagerClient>();
    private static ConcurrentHashMap<String, ProducerManagerClient> _producerManagerClientMap = new ConcurrentHashMap<String, ProducerManagerClient>();
    private static ConcurrentHashMap<String, CacheClient> _cacheClientMap = new ConcurrentHashMap<String, CacheClient>();
    private static ConcurrentHashMap<String, RedisCacheClient> redisCacheClientMap = new ConcurrentHashMap<String, RedisCacheClient>();

    public static synchronized CacheManagerClient getCacheManagerClient(
	    String zkAddress) {
	_logger.info("ClientFactory.getCacheManagerClient zkAddress "
		+ zkAddress);
	CacheManagerClient cc = _cacheManagerClientMap.get(zkAddress);
	if (null == cc) {
	    _logger.info("ClientFactory.getCacheManagerClient new CacheManagerClient for zkAddress "
		    + zkAddress);
	    cc = new CacheManagerClient(zkAddress);
	    _cacheManagerClientMap.put(zkAddress, cc);
	}
	return cc;
    }

    public static synchronized ProducerManagerClient getProducerManagerClient(
	    String zkAddress) {
	_logger.info("ClientFactory.getProducerManagerClient zkAddress "
		+ zkAddress);
	ProducerManagerClient pc = _producerManagerClientMap.get(zkAddress);
	if (null == pc) {
	    _logger.info("ClientFactory.getProducerManagerClient new ProducerManagerClient for zkAddress "
		    + zkAddress);
	    pc = new ProducerManagerClient(zkAddress);
	    _producerManagerClientMap.put(zkAddress, pc);
	}
	return pc;
    }

    public static synchronized CacheClient getCachegClient(String zkAddress) {
	_logger.info("ClientFactory.getCacheClient zkAddress " + zkAddress);
	CacheClient cc = _cacheClientMap.get(zkAddress);
	if (null == cc) {
	    _logger.info("ClientFactory.getCacheClient new CacheClient for zkAddress "
		    + zkAddress);
	    cc = new CacheClient(zkAddress);
	    _cacheClientMap.put(zkAddress, cc);
	}
	return cc;
    }

    /**
     * 
     * @param zkAddress
     * @return
     */
    public static synchronized RedisCacheClient getRedisCachegClient(
	    String zkAddress) {
	_logger.info("ClientFactory.getRedisCacheClient zkAddress " + zkAddress);
	RedisCacheClient rcc = redisCacheClientMap.get(zkAddress);
	if (null == rcc) {
	    _logger.info("ClientFactory.getRedisCacheClient new CacheClient for zkAddress "
		    + zkAddress);
	    rcc = new RedisCacheClient(zkAddress);
	    redisCacheClientMap.put(zkAddress, rcc);
	}
	return rcc;
    }

}