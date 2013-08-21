package xce.tripod;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.xce.log.Oce;

/**
 * 对外提供的Client接口
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class TripodClient {

    protected String namespaceId;

    private String businessId;

    private CacheProxy cacheProxy;

    private CacheManagerProxy cacheManagerProxy;

    public TripodClient(String namespaceId, String businessId) {
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        cacheProxy = new CacheProxy(namespaceId, businessId);
        cacheManagerProxy = new CacheManagerProxy(namespaceId, businessId);
    }

    /**
     * 对外提供的get接口 secs为过期时间
     * 
     * @param key
     * @param secs
     * @param useMaster 是否走主库
     * @return
     * @throws TripodException
     */
    public byte[] get(String key, int secs, boolean useMaster) throws TripodException {
        RemoteLogger.getInstance().log(dump("get"));

        byte[] value = null;
        value = cacheProxy.get(key);
        if (value == null || value.length == 0) {
            value = cacheManagerProxy.get(key, secs, useMaster);
        }
        return value;
    }

    public byte[] get(String key, int secs) throws TripodException {
        return get(key, secs, false);
    }

    /**
     * 对外提供的批量get接口 secs为过期时间
     * 
     * @param keys
     * @param secs
     * @return
     * @throws TripodException
     */
    public Map<String, byte[]> multiGet(List<String> keys, int secs, boolean useMaster)
            throws TripodException {
        RemoteLogger.getInstance().log(dump("multiGet"));
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        result = cacheProxy.multiGet(keys);

        for (Map.Entry<String, byte[]> m : result.entrySet()) {
            if (m.getValue() == null || m.getValue().length == 0) {
                String tempKey = m.getKey();
                try {
                    result.put(tempKey, cacheManagerProxy.get(tempKey, secs, useMaster));
                } catch (Ice.TimeoutException e) {
                    Oce.getLogger().error(
                            this.getClass().getName() + " .cacheManagerProxy get  " + tempKey);
                }
            }
        }

        return result;
    }

    public Map<String, byte[]> multiGet(List<String> keys, int secs) throws TripodException {
        return multiGet(keys, secs, false);
    }

    /**
     * 对外提供的批量get接口,允许miss
     * 
     * @param keys
     * @param secs
     * @return
     * @throws TripodException
     */
    public Map<String, byte[]> multiGetAndIngoreMiss(List<String> keys) throws TripodException {
        RemoteLogger.getInstance().log(dump("multiGetAndIngoreMiss"));
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        result = cacheProxy.multiGet(keys);
        return result;
    }

    /**
     * 对外提供的getAndIgnoreMiss, 允许miss的
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    public byte[] getAndIgnoreMiss(String key) throws TripodException {
        RemoteLogger.getInstance().log(dump("getAndIgnoreMiss"));
        byte[] value = cacheProxy.get(key);
        return value;
    }

    /**
     * 对外提供的load接口
     * 
     * @param key
     * @param secs
     * @param useMaster
     * @return
     * @throws TripodException 
     */
    public boolean load(String key, int secs, boolean useMaster) throws TripodException {
        RemoteLogger.getInstance().log(dump("load"));
        return cacheManagerProxy.load(key, secs, useMaster);
    }

    /**
     * 对于Hash结构的load接口
     * 
     * @param key
     * @param secs
     * @param useMaster
     * @return
     */
    public boolean loadHash(String key, int secs, boolean useMaster) {
        RemoteLogger.getInstance().log(dump("loadHash"));
        return cacheManagerProxy.loadHash(key, secs, useMaster);
    }

    /**
     * 对外提供的remove接口
     * 
     * @param key
     * @return
     * @throws TripodException 
     */
    public boolean remove(String key) throws TripodException {
        RemoteLogger.getInstance().log(dump("remove"));
        return cacheManagerProxy.remove(key);
    }

    /**
     * 对外提供的set接口
     * 
     * @param key
     * @param value
     * @param secs
     * @return
     * @throws TripodException 
     */
    public boolean set(String key, byte[] value, int secs) throws TripodException {
        RemoteLogger.getInstance().log(dump("set"));
        return cacheManagerProxy.set(key, value, secs);
    }

    /**
     * 对外提供的inc接口
     * 
     * @param step 增量，可以是负数，值域为long
     * @return 成功返回inc后的String值，失败返回空字符串
     * @throws TripodException 
     */
    public String inc(String key, String step) throws TripodException {
        RemoteLogger.getInstance().log(dump("inc"));
        return cacheManagerProxy.inc(key, step);
    }

    public String getBusinessId() {
        return businessId;
    }

    /**
     * dump info
     * 
     * @return
     */
    public String dump(String op) {
        StringBuffer sb = new StringBuffer();
        sb.append("TripodClient.").append(op).append("|").append(Constants.VERSION).append("|")
                .append(namespaceId).append("|").append(businessId);
        return sb.toString();
    }

    /**
     * 取得cache中存储Hash结构的指定key的整张表
     * 
     * @param key
     * @param secs
     * @return
     * @throws TripodException
     */

    public Map<String, String> getAllHash(String key, int secs, boolean useMaster)
            throws TripodException {
        RemoteLogger.getInstance().log(dump("getAllHash"));
        boolean exists = cacheProxy.exists(key);
        Map<String, String> hash_value = null;
        if (exists) {
            hash_value = cacheProxy.getAllHash(key);
        } else {
            hash_value = cacheManagerProxy.getAllHash(key, secs, useMaster);
        }
        return hash_value;
    }

    public Map<String, String> getAllHash(String key, int secs) throws TripodException {
        return getAllHash(key, secs, false);
    }

    /**
     * 允许miss的情况
     * 
     * @param key
     * @return
     */

    public Map<String, String> getAllHashAndIgnoreMiss(String key) {
        RemoteLogger.getInstance().log(dump("getAllHashAndIgnoreMiss"));
        Map<String, String> result = cacheProxy.getAllHash(key);
        return result;
    }

    /**
     * 添加部分field的setHash方法
     * 
     * @param key
     * @param hash_value
     * @return
     */

    public boolean setPartHash(String key, Map<String, String> hash_value) {
        RemoteLogger.getInstance().log(dump("setPartHash"));
        return cacheManagerProxy.setPartHash(key, hash_value);
    }

    /**
     * 部分fields的删除接口
     * 
     * @param key
     * @param hash_fields
     * @return
     */
    public boolean deletePartHash(String key, String[] hash_fields) {
        RemoteLogger.getInstance().log(dump("deletePartHash"));
        return cacheManagerProxy.deletePartHash(key, hash_fields);
    }

}
