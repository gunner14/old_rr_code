package xce.tripod;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import xce.tripod.util.Continuum;

public class CacheProxy extends ZNodeListener implements CacheClient {

    private String namespaceId;

    private String businessId;

    private Continuum continuum;

    private int connTimeOutInMilliseconds;

    private Map<String, JedisPool> cacheClientPool; // nick to pool

    private ReentrantReadWriteLock rwLock;

    public CacheProxy(String ns, String biz, int connTimeOutInMilliseconds) {
        super(Constants.DEFAULT_ZK_CACHE_ZNODE + Constants.SEPARATOR + ns);
        this.namespaceId = ns;
        this.businessId = biz;
        this.connTimeOutInMilliseconds = connTimeOutInMilliseconds;
        continuum = new Continuum(biz);
        cacheClientPool = new HashMap<String, JedisPool>();
        rwLock = new ReentrantReadWriteLock();
        try {
            ZkClient zkClient = ZkClient.getInstance(Constants.DEFAULT_ZK_ADDRESS);
            zkClient.addZnodeListener(this);
            List<String> childrenNames = zkClient.getNodes(getZNode());
            update(childrenNames);
        } catch (TripodException e) {
            e.printStackTrace();
        }
    }

    protected static String generateCacheKey(String prefix, String key) {
        final String separator = ":";
        return prefix + separator + key;
    }

    protected JedisPool locateCacheClientPool(String key) {
        String nickname = continuum.locate(Continuum.hash(key));
        ReadLock rLock = rwLock.readLock();
        try {
            rLock.lock();
            return cacheClientPool.get(nickname);
        } finally {
            rLock.unlock();
        }
    }

    protected String description(String desc) {
        StringBuffer sb = new StringBuffer();
        sb.append("[ns:").append(namespaceId).append(",biz:").append(businessId);
        if (desc != null) {
            sb.append(", msg:").append(desc);
        }
        sb.append("]");
        return sb.toString();
    }

    @Override
    public byte[] get(String key) throws TripodException {
        String cacheKey = generateCacheKey(businessId, key);
        // System.out.println(cacheKey);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        Jedis jedis = null;
        byte[] value = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                // System.out.println(jedis.getClient().getHost() + ":" +
                // jedis.getClient().getPort());
                value = jedis.get(cacheKey.getBytes());
            }
            pool.returnResource(jedis);
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
        return value;
    }

    public Map<String, byte[]> multiGet(List<String> keys) throws TripodException {
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        Map<JedisPool, List<String>> map = new HashMap<JedisPool, List<String>>();
        for (String tempKey : keys) {
            JedisPool pool = locateCacheClientPool(tempKey);
            if (pool == null) {
                System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                        + namespaceId + "\tbusinessId:" + businessId);
                result.put(tempKey, null);
            } else {
                if (map.containsKey(pool)) {
                    map.get(pool).add(tempKey);
                } else {
                    List<String> poolKeys = new LinkedList<String>();
                    poolKeys.add(tempKey);
                    map.put(pool, poolKeys);
                }
            }
        }
        // System.out.println("map size" + map.size());
        for (Map.Entry<JedisPool, List<String>> m : map.entrySet()) {
            JedisPool pool = m.getKey();
            List<String> poolKeys = m.getValue();
            // System.out.println("each map keys " + poolKeys.size() +
            // poolKeys);
            Jedis jedis = null;
            List<byte[]> values = null;
            try {
                jedis = pool.getResource();
                String[] cacheKeys = new String[poolKeys.size()];
                byte[][] cacheByteKeys = new byte[poolKeys.size()][];
                Iterator<String> iter = poolKeys.iterator();
                for (int i = 0; iter.hasNext(); ++i) {
                    cacheKeys[i] = generateCacheKey(businessId, iter.next());
                    cacheByteKeys[i] = cacheKeys[i].getBytes();
                }

                if (jedis != null) {
                    values = jedis.mget(cacheByteKeys);
                    Iterator<byte[]> valueIter = values.iterator();
                    Iterator<String> keyIter = poolKeys.iterator();
                    while (valueIter.hasNext() && keyIter.hasNext()) {
                        result.put(keyIter.next(), valueIter.next());
                    }
                }
                pool.returnResource(jedis);
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
        }
        return result;
    }

    public List<String> zRange(String key, int begin, int limit) throws TripodException {
        if (begin < 0) {
            return null;
        }
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.zRange cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        Jedis jedis = null;
        List<String> result = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                result = new ArrayList<String>();
                int stopPos = begin + limit - 1;
                Set<String> tempResult = null;
                if (limit == 0) {
                    pool.returnResource(jedis);
                    return result;
                } else if (limit < 0) {
                    tempResult = jedis.zrange(cacheKey, begin, limit);
                } else {
                    tempResult = jedis.zrange(cacheKey, begin, stopPos);
                }
                for (String temp : tempResult) {
                    result.add(temp);
                }
            }
            pool.returnResource(jedis);
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

    @Override
    public List<String> getList(String key, int begin, int limit) throws TripodException {
        if (begin < 0) {
            return null;
        }
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        Jedis jedis = null;
        List<String> result = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                result = new LinkedList<String>();
                if (limit == 0) {
                    pool.returnResource(jedis);
                    return result;
                }
                int startPos = begin;
                int stopPos = startPos + limit - 1;
                int batchEnd;
                while (true) {
                    batchEnd = startPos + Constants.REDIS_BATCH_SIZE - 1;
                    if (limit > 0) {
                        batchEnd = batchEnd < stopPos ? batchEnd : stopPos;
                    }
                    List<String> fetch = jedis.lrange(cacheKey, startPos, batchEnd);
                    int fetchSize = fetch.size();
                    result.addAll(fetch);
                    if (fetchSize == Constants.REDIS_BATCH_SIZE) {
                        startPos = ++batchEnd;
                    }
                    if (fetchSize >= 0 && fetchSize < Constants.REDIS_BATCH_SIZE) {
                        break;
                    }
                }
            }
            pool.returnResource(jedis);
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

    @Override
    public Map<String, String> getHash(String key, List<String> fields) throws TripodException {
        if (fields.isEmpty()) {
            return null;
        }
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        Jedis jedis = null;
        Map<String, String> result = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                List<String> values = jedis.hmget(cacheKey,
                        (String[]) fields.toArray(new String[0]));
                Iterator<String> valueIter = values.iterator();
                Iterator<String> fieldIter = fields.iterator();
                result = new HashMap<String, String>();
                while (valueIter.hasNext() && fieldIter.hasNext()) {
                    result.put(fieldIter.next(), valueIter.next());
                }
            }
            pool.returnResource(jedis);
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

    @Override
    public Map<String, String> getAllHash(String key) throws TripodException {
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.getAllHash cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return null;
        }
        Jedis jedis = null;
        Map<String, String> result = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                result = jedis.hgetAll(cacheKey);
            }
            pool.returnResource(jedis);
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

    @Override
    public boolean exists(String key) throws TripodException {
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return false;
        }
        boolean result = false;
        Jedis jedis = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                result = jedis.exists(cacheKey);
            }
            pool.returnResource(jedis);
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

    @Override
    public Long size(String key) throws TripodException {
        String cacheKey = generateCacheKey(businessId, key);
        JedisPool pool = locateCacheClientPool(key);
        if (pool == null) {
            System.err.println("CacheProxy.get cannot get client pool!" + "\tnamespaceId:"
                    + namespaceId + "\tbusinessId:" + businessId);
            return (long) -1;
        }
        Jedis jedis = null;
        String valueType = null;
        Long value = null;
        try {
            jedis = pool.getResource();
            if (jedis != null) {
                valueType = jedis.type(cacheKey);
                if (valueType.equals(Constants.REDIS_STRING_TYPE)) {
                    value = jedis.strlen(cacheKey);
                } else if (valueType.equals(Constants.REDIS_LIST_TYPE)) {
                    value = jedis.llen(cacheKey);
                } else if (valueType.equals(Constants.REDIS_HASH_TYPE)) {
                    value = jedis.hlen(cacheKey);
                }
            }
            pool.returnResource(jedis);
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
        return value;
    }

    @Override
    public boolean update(List<String> childrenNameList) {
        WriteLock wLock = rwLock.writeLock();
        try {
            wLock.lock();
            Continuum cm = new Continuum(businessId);
            Map<String, JedisPool> pool = new HashMap<String, JedisPool>();
            for (String childName : childrenNameList) {
                String[] units = childName.split(":");
                if (units.length == 3) {
                    String nickname = units[0];
                    String address = units[1];
                    int port = 0;
                    try {
                        port = Integer.parseInt(units[2]);
                    } catch (Exception e) {
                        e.printStackTrace();
                        continue;
                    }
                    pool.put(nickname,
                            JedisPoolFactory.getPool(address, port, connTimeOutInMilliseconds));
                    cm.add(nickname, 100);
                }
            }

            if (!pool.isEmpty()) {
                cm.rebuild();
                continuum = cm;
                cacheClientPool.clear();
                cacheClientPool.putAll(pool);
            }
        } finally {
            wLock.unlock();
        }
        return true;
    }
}
