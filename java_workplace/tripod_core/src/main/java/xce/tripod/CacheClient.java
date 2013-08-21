package xce.tripod;

import java.util.List;
import java.util.Map;

/**
 * CacheClient <br/>
 * 接口定义
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public interface CacheClient {

    /**
     * get接口， 返回String,如果失败返回null
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    byte[] get(String key) throws TripodException;

    /**
     * 批量get接口， 返回key-value map,如果失败对应的值返回null
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    Map<String, byte[]> multiGet(List<String> keys) throws TripodException;

    /**
     * 是否存在接口, 成功返回true,失败返回false
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    boolean exists(String key) throws TripodException;

    /**
     * 取value的大小接口
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    Long size(String key) throws TripodException;

    /**
     * get list接口，支持分页, 如果失败，返回空列表
     * 
     * @param key
     * @param begin
     * @param limit
     * @return
     */
    List<String> getList(String key, int begin, int limit);

    /**
     * get hash接口，如果失败，返回空map
     * 
     * @param key
     * @param fields
     * @return
     */
    Map<String, String> getHash(String key, List<String> fields);

    /**
     * get all hash, 如果失败，返回空map
     * 
     * @param key
     * @return
     */
    Map<String, String> getAllHash(String key);
}
