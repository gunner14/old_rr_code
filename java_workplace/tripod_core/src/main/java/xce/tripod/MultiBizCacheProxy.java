package xce.tripod;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

public class MultiBizCacheProxy extends CacheProxy {
    public MultiBizCacheProxy(String ns) {
	super(ns, "");
    }

    public Map<String, byte[]> multiBizGet(String key, List<String> bizs) {
	Map<String, byte[]> result = new HashMap<String, byte[]>();

	String[] cacheKeys = new String[bizs.size()];
	byte[][] cacheByteKeys = new byte[cacheKeys.length][];

	Iterator<String> iter = bizs.iterator();
	for (int i = 0; iter.hasNext(); ++i) {
	    cacheKeys[i] = generateCacheKey(iter.next(), key);
	    // System.ou t.println(cacheKeys[i]);
	    cacheByteKeys[i] = cacheKeys[i].getBytes();
	}

	JedisPool pool = locateCacheClientPool(key);
	if (pool == null) {
	    System.err.println("CacheProxy.get cannot get client pool!");
	    return null;
	}
	Jedis jedis = null;
	List<byte[]> values = null;
	try {
	    jedis = pool.getResource();
	    if (jedis != null) {
		// System.out.println("*****redis address*****");
		// System.out.println(jedis.getClient().getHost() + ":"
		// + jedis.getClient().getPort());

		values = jedis.mget(cacheByteKeys);
	    }
	    pool.returnResource(jedis);

	    Iterator<byte[]> valueIter = values.iterator();
	    Iterator<String> bizIter = bizs.iterator();
	    while (valueIter.hasNext() && bizIter.hasNext()) {
		result.put(bizIter.next(), valueIter.next());
	    }

	} catch (Exception e) {
	    e.printStackTrace();
	    if (jedis != null) {
		pool.returnBrokenResource(jedis);
	    }
	}
	return result;
    }

}
