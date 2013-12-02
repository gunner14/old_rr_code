package xce.tripod;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

public class MultiBizCacheProxy extends CacheProxy {

    public MultiBizCacheProxy(String ns, int connTimeOutInMilliseconds) {
        super(ns, "", connTimeOutInMilliseconds);
    }

    public Map<String, byte[]> multiBizGet(String key, List<String> bizs) throws TripodException {
        Map<String, byte[]> result = new HashMap<String, byte[]>();

        String[] cacheKeys = new String[bizs.size()];
        byte[][] cacheByteKeys = new byte[cacheKeys.length][];

        Iterator<String> iter = bizs.iterator();
        for (int i = 0; iter.hasNext(); ++i) {
            cacheKeys[i] = generateCacheKey(iter.next(), key);
            cacheByteKeys[i] = cacheKeys[i].getBytes();
        }

        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            throw new TripodException(description("locateCacheClientPool fail MultiBizCacheProxy.multiBizGet " + key));
        }
        Jedis jedis = null;
        List<byte[]> values = null;
        try {
            jedis = pool.getResource();
            values = jedis.mget(cacheByteKeys);
            pool.returnResource(jedis);

            Iterator<byte[]> valueIter = values.iterator();
            Iterator<String> bizIter = bizs.iterator();
            while (valueIter.hasNext() && bizIter.hasNext()) {
                result.put(bizIter.next(), valueIter.next());
            }
        } catch (Exception e) {
            if (jedis != null) {
                try {
                    pool.returnBrokenResource(jedis);
                } catch (Exception e1) {
                    throw new TripodException(description(e1.getMessage()), e1);
                }
            }
            throw new TripodException(description(e.getMessage()), e);
        }
        return result;
    }

}
