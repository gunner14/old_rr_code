package xce.tripod;

import java.util.HashMap;

import redis.clients.jedis.JedisPool;

import org.apache.commons.pool.impl.GenericObjectPool.Config;

/**
 * JedisPoolFactory
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class JedisPoolFactory {

    private static HashMap<String, JedisPool> poolMap = new HashMap<String, JedisPool>();

    public static JedisPool getPool(String address, int port) {
	synchronized (JedisPoolFactory.class) {
	    String key = address + ":" + port;
	    JedisPool pool = poolMap.get(key);
	    if (pool == null) {
                pool = new JedisPool(new Config(), address, port, 1000);
		poolMap.put(key, pool);
	    }
	    return pool;
	}
    }

}
