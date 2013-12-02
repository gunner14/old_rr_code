package xce.tripod;

import redis.clients.jedis.JedisPool;
import redis.clients.jedis.exceptions.JedisConnectionException;

/**
 * TripodException会包装tripod底层异常，应用层应该catch住此异常
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class TripodException extends Exception {

    private static final long serialVersionUID = 1L;

    public TripodException(String msg) {
        super(msg);
    }

    public TripodException(String msg, Throwable t) {
        super(msg, t);
    }

    public TripodException(Throwable cause) {
        super(cause);
    }
}
