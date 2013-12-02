package xce.tripod;

import java.util.HashMap;

import org.apache.commons.pool.impl.GenericObjectPool.Config;

import redis.clients.jedis.JedisPool;

/**
 * JedisPoolFactory
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class JedisPoolFactory {

    private static HashMap<String, JedisPool> poolMap = new HashMap<String, JedisPool>();

    public static JedisPool getPool(String address, int port, int connTimeOutInMilliseconds) {
	synchronized (JedisPoolFactory.class) {
	    String key = address + ":" + port;
	    JedisPool pool = poolMap.get(key);
	    if (pool == null) {
                pool = new JedisPool(new Config(), address, port, connTimeOutInMilliseconds);
		poolMap.put(key, pool);
	    }
	    return pool;
	}
    }

}
