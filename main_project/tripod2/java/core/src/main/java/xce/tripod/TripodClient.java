package xce.tripod;

import java.util.ArrayList;
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
        this(namespaceId, businessId, 1000, 1000);
    }

    public TripodClient(String namespaceId, String businessId, int readTimeOutInMilliseconds,
            int writeTimeOutInMilliseconds) {
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        cacheProxy = new CacheProxy(namespaceId, businessId, readTimeOutInMilliseconds);
        cacheManagerProxy = new CacheManagerProxy(namespaceId, businessId,
                writeTimeOutInMilliseconds);
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
        InvokeClient.getInstance().log("get", namespaceId, businessId);
        byte[] value = null;
        //对于Cache的get失败，认为是Cache不可用，继续从CacheManager中get
        try {
            value = cacheProxy.get(key);
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".get " //NL
                    + key + " TripodException " + e.toString());
        }
        if (value == null || value.length == 0) {
            value = cacheManagerProxy.get(key, secs, useMaster);
            InvokeClient.getInstance().log("miss", namespaceId, businessId);
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
        InvokeClient.getInstance().log("multiGet", namespaceId, businessId, keys.size());
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        try {
            result = cacheProxy.multiGet(keys);
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".getList " //NL
                    + keys.size() + " TripodException " + e.toString());
        }
        
        long missCount = 0L;
        for (String tempKey : keys) {
            if (result.get(tempKey) == null || result.get(tempKey).length == 0) {
                ++missCount;
                try {
                    result.put(tempKey, cacheManagerProxy.get(tempKey, secs, useMaster));
                } catch (Ice.TimeoutException e) {
                    Oce.getLogger().error(this.getClass().getName() //NL
                            + " .cacheManagerProxy get " + tempKey + " " + e.toString());
                }
            }
        }
        InvokeClient.getInstance().log("miss_multiGet", namespaceId, businessId, missCount);
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
        InvokeClient.getInstance().log("multiGetAndIngoreMiss", namespaceId, businessId,
                keys.size());
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        result = cacheProxy.multiGet(keys);
        long missCount = 0L;
        for (Map.Entry<String, byte[]> m : result.entrySet()) {
            if (m.getValue() == null || m.getValue().length == 0) {
                ++missCount;
            }
        }
        InvokeClient.getInstance().log("miss_multiGetAndIngoreMiss", namespaceId, businessId,
                missCount);
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
        InvokeClient.getInstance().log("getAndIgnoreMiss", namespaceId, businessId);
        byte[] value = cacheProxy.get(key);
        if (value == null || value.length == 0) {
            InvokeClient.getInstance().log("miss", namespaceId, businessId);
        }
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
        InvokeClient.getInstance().log("load", namespaceId, businessId);
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
    public boolean loadHash(String key, int secs, boolean useMaster) throws TripodException {
        InvokeClient.getInstance().log("loadHash", namespaceId, businessId);
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
        InvokeClient.getInstance().log("remove", namespaceId, businessId);
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
        InvokeClient.getInstance().log("set", namespaceId, businessId);
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
        InvokeClient.getInstance().log("inc", namespaceId, businessId);
        return cacheManagerProxy.inc(key, step);
    }

    public String getBusinessId() {
        return businessId;
    }

    /**
     * getList 接口
     * @param key
     * @param begin getlist的起始位置 非负
     * @param limit 取的limit数量，limit为-1时取得list长度为begin开始到list末尾
     * @param secs
     * @param useMaster
     * @return 返回值为取得到的list，处理失败时返回null，数据不存在返回空List
     * @throws TripodException 
     */
    public List<String> getList(String key, int begin, int limit, int secs, boolean useMaster)
            throws TripodException {
        InvokeClient.getInstance().log("getList", namespaceId, businessId);
        List<String> result = null;
        try {
            result = cacheProxy.getList(key, begin, limit);
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".getList " //NL
                    + key + " TripodException " + e.toString());
        }
        if (result == null || result.size() == 0) {
            result = cacheManagerProxy.getList(key, begin, limit, secs, useMaster);
        }
        return result;
    }
    /**
     * loadList 接口
     * @param key
     * @param secs
     * @param useMaster
     * @return
     * @throws TripodException 
     */
    public boolean loadList(String key, int secs, boolean useMaster) throws TripodException {
        InvokeClient.getInstance().log("loadList", namespaceId, businessId);
        return cacheManagerProxy.loadList(key, secs, useMaster);
    }
    /**
     * setList 接口
     * @param key
     * @param value
     * @param secs
     * @return
     * @throws TripodException 
     */
    
    public boolean setList(String key, List<String> value, int secs) throws TripodException {
        InvokeClient.getInstance().log("setList", namespaceId, businessId);
        return cacheManagerProxy.setList(key, value, secs);
    }
    
    /**
     * getList 仅从cache中取出的方法
     * @param key
     * @param begin
     * @param limit
     * @return
     * @throws TripodException 
     */
    public List<String> getListIgnoreMiss(String key, int begin, int limit) throws TripodException {
        InvokeClient.getInstance().log("getListIgnoreMiss", namespaceId, businessId);
        return cacheProxy.getList(key, begin, limit);
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
        InvokeClient.getInstance().log("getAllHash", namespaceId, businessId);
        boolean exists = false;
        Map<String, String> hash_value = null;
        try {
            exists = cacheProxy.exists(key);
            if(exists){
                hash_value = cacheProxy.getAllHash(key);
            }
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".getAllHash " //NL
                    + key + " TripodException " + e.toString());
        }
        if (!exists) {
            hash_value = cacheManagerProxy.getAllHash(key, secs, useMaster);
        }
        return hash_value;
    }

    public Map<String, String> getAllHash(String key, int secs) throws TripodException {
        return getAllHash(key, secs, false);
    }

    /**
     * 仅从Cache中取整个hash列表
     * 
     * @param key
     * @return
     * @throws TripodException 
     */

    public Map<String, String> getAllHashAndIgnoreMiss(String key) throws TripodException {
        InvokeClient.getInstance().log("getAllHashAndIgnoreMiss", namespaceId, businessId);
        Map<String, String> result = cacheProxy.getAllHash(key);
        return result;
    }

    /**
     * 添加部分field的setHash方法
     * 
     * @param key
     * @param hash_value
     * @return
     * @throws TripodException 
     */

    public boolean setPartHash(String key, Map<String, String> hash_value) throws TripodException {
        InvokeClient.getInstance().log("setPartHash", namespaceId, businessId);
        return cacheManagerProxy.setPartHash(key, hash_value);
    }

    /**
     * 部分fields的删除接口
     * 
     * @param key
     * @param hash_fields
     * @return
     * @throws TripodException 
     */
    public boolean deletePartHash(String key, String[] hash_fields) throws TripodException {
        InvokeClient.getInstance().log("deletePartHash", namespaceId, businessId);
        return cacheManagerProxy.deletePartHash(key, hash_fields);
    }
    /**
     * zadd接口 添加sorted set结构到Cache中，若当前key不存在cache中则创建
     * （为sorted set 结构的追加方法即在key所对应的原有数据中添加相应的数据并按照set_with_scores的first参数进行排序）。
     * @param key
     * @param set_with_scores的key域用于排序，类型为long，set_with_scores的value域为插入value
     * @return
     * @throws TripodException
     */
    public boolean zAdd(String key, Map<String, String> set_with_scores) throws TripodException {
        InvokeClient.getInstance().log("zAdd", namespaceId, businessId);
        return cacheManagerProxy.zAdd(key, set_with_scores);
    }
    /**
     * zRemRangeByRank接口 删除 key对应的sorted set结构中位于begin到end中的部分
     * @param key
     * @param begin 起始位置，非负
     * @param end 终止位置，闭区间，应大于begin（-1时删除由begin开始到末尾的部分）
     * @return
     * @throws TripodException
     */
    public boolean zRemRangeByRank(String key, int begin, int end) throws TripodException {
        InvokeClient.getInstance().log("zRemRangeByRank", namespaceId, businessId);
        if (begin < 0) {
            return false;
        }
        return cacheManagerProxy.zRemRangeByRank(key, begin, end);
    }
    
    /**
     * zRemRangeByScore接口，根据score值删除sorted set中满足条件的元素
     * Removes all elements in the sorted set stored at key with a score between min and max (inclusive)
     * @param key
     * @param min  要删除元素score的最小值,闭区间
     * @param max  要删除元素score的最大值,闭区间
     * @return
     * @throws TripodException
     */
    public boolean zRemRangeByScore(String key, String min, String max) throws TripodException {
        InvokeClient.getInstance().log("zRemRangeByScore", namespaceId, businessId);
        return cacheManagerProxy.zRemRangeByScore(key, min, max);
    }
    
    /**
     * zRange接口 从cache中取begin位置开始limit为范围的大小sorted set结构的数据
     * @param key
     * @param begin 起始位置，非负
     * @param limit 限制大小，为-1时候返回有begin开始的全部数据
     * @return 返回cache中数据，处理失败是返回null，无数据是返回空List
     * @throws TripodException
     */
    public List<String> zRange(String key, int begin, int limit) throws TripodException {
        InvokeClient.getInstance().log("zRange", namespaceId, businessId);
        List<String> result = new ArrayList<String>();
        result = cacheProxy.zRange(key, begin, limit);
        return result;
    }
    
    /**
     * zSet接口 覆写方法将传进去的set_with_scores替换原来的cache中对应key的相应数据
     * @param key
     * @param set_with_scores 它的first域用来比较需由long类型转成，second域为存储内容
     * @param secs
     * @return
     * @throws TripodException
     */
    public boolean zSet(String key, Map<String, String> set_with_scores, int secs) throws TripodException {
        InvokeClient.getInstance().log("zSet", namespaceId, businessId);
        return cacheManagerProxy.zSet(key, set_with_scores, secs);
    }
    /**
     * size接口
     * @param key
     * @return
     * @throws TripodException 
     */
    public Long size(String key) throws TripodException {
        InvokeClient.getInstance().log("size", namespaceId, businessId);
        boolean exists = cacheProxy.exists(key);
        if (exists) {
        	return cacheProxy.size(key);
        } else {
        	return (long) 0;
        } 
    }

}
