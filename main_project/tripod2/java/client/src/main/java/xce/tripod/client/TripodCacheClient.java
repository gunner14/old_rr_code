package xce.tripod.client;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import xce.tripod.TripodClient;
import xce.tripod.TripodException;
import xce.tripod.util.TripodUtil;

/**
 * TripodCacheClient 提供给网站技术同学使用
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class TripodCacheClient {

    public static final long ERROR_COUNT = -1;

    private String namespaceId;

    private String businessId;

    private TripodClient tripodClient;

    TripodCacheClient(String namespaceId, String businessId) {
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        tripodClient = new TripodClient(namespaceId, businessId);
    }

    //    TripodCacheClient(String namespaceId, String businessId, int readTimeOutInMilliseconds,
    //            int writeTimeOutInMilliseconds) {
    //        this.namespaceId = namespaceId;
    //        this.businessId = businessId;
    //        tripodClient = new TripodClient(namespaceId, businessId, readTimeOutInMilliseconds,
    //                writeTimeOutInMilliseconds);
    //    }

    /**
     * 写 接口
     * 
     * @param key
     * @param value 必须实现java.io.Serializable
     * @param expireTime
     * @throws TripodException 
     */
    public void set(String key, Object value, int expireTime) throws TripodException {
        byte[] byteData = null;
        try {
            byteData = TripodUtil.serialize(value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (byteData != null && byteData.length != 0) {
            tripodClient.set(key, byteData, expireTime);
        }
    }

    /**
     * 读 接口
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    public Object get(String key) throws TripodException {
        Object value = null;
        byte[] byteValue = tripodClient.getAndIgnoreMiss(key);
        if (byteValue == null || byteValue.length == 0) {
            return value;
        }
        try {
            value = TripodUtil.deserialize(byteValue);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return value;
    }

    /**
     * 删除接口
     * 
     * @param key
     * @throws TripodException
     */

    public void remove(String key) throws TripodException {
        tripodClient.remove(key);
    }

    /**
     * 批量取接口
     * 
     * @param keyList
     * @return
     * @throws TripodException
     */
    public Map<String, Object> multiGet(List<String> keyList) throws TripodException {
        Map<String, Object> result = new HashMap<String, Object>();
        Map<String, byte[]> dataMap = tripodClient.multiGetAndIngoreMiss(keyList);
        if (!dataMap.isEmpty()) {
            for (Entry<String, byte[]> entry : dataMap.entrySet()) {
                String k = entry.getKey();
                byte[] data = entry.getValue();
                try {
                    if (data != null && data.length != 0) {
                        Object obj = TripodUtil.deserialize(data);
                        result.put(k, obj);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    /** binary接口BEGIN */
    /** ------------------------------------------------------- */

    /**
     * binary写 接口
     * @param key
     * @param value 必须实现java.io.Serializable
     * @param expireTime
     * @throws TripodException
     */
    public boolean binarySet(String key, byte[] value, int expireTime) throws TripodException {
        if (value != null && value.length != 0) {
            return tripodClient.set(key, value, expireTime);
        }
        return false;
    }

    /**
     * binary读 接口
     * @param key
     * @return
     * @throws TripodException
     */
    public byte[] binaryGet(String key) throws TripodException {
        return tripodClient.getAndIgnoreMiss(key);
    }

    /**
     * binary批量取接口
     * @param keyList
     * @return
     * @throws TripodException
     */
    public Map<String, byte[]> binaryMultiGet(List<String> keyList) throws TripodException {
        return tripodClient.multiGetAndIngoreMiss(keyList);
    }
    
    /** ------------------------------------------------------- */
    /** binary接口END */


    /** 计数器接口BEGIN */
    /** ------------------------------------------------------- */

    /**
     * 计数器初始化接口
     * 
     * @param key
     * @param value 初始值
     * @param expireTime 过期时间,必须设置过期时间，不能永不过期，否则会被认为无效
     * @throws TripodException
     */
    public void setLong(String key, long value, int expireTime) throws TripodException {
        tripodClient.set(key, String.valueOf(value).getBytes(), expireTime);
    }

    /**
     * 计数器读接口
     * 
     * @param key
     * @return 计数器只支持非负整数，成功返回正常值，失败返回ERROR_COUNT
     * @throws TripodException
     */
    public long getLong(String key) throws TripodException {
        long result = ERROR_COUNT;
        byte[] byteValue = tripodClient.getAndIgnoreMiss(key);
        if (byteValue != null && byteValue.length > 0) {
            try {
                result = Long.parseLong(new String(byteValue));
            } catch (Exception e) {
                throw new TripodException(e);
            }
        }
        return result;
    }

    /**
     * 计数器增减接口
     * 1. 计数器只支持非负整数，成功返回正常值，异常返回ERROR_COUNT
     * 2. 在调用之前，需要用[计数器初始化接口setLong]来初始化指定过期时间，否则会自动生成一个初值为0且永不过期的值
     * 3. 如果调用失败，需要用[计数器初始化接口setLong] 重新设值，否则后续incLong和get调用会一直失败！！
     *
     * @param key
     * @param step 增量。可以是负数，值域为long
     * @return 增加后的值。失败返回ERROR_COUNT
     * @throws TripodException
     */
    public long incLong(String key, long step) throws TripodException {
        long result = ERROR_COUNT;
        String data = tripodClient.inc(key, String.valueOf(step));
        if (data != null && data.length() > 0) {
            result = Long.parseLong(data);
        }
        return result;
    }

    /** ------------------------------------------------------- */
    /** 计数器接口END */

    
    /** 有序集合接口BEGIN */
    /** ------------------------------------------------------- */
    
    /**
     * 有序集合初始化写接口，它用参数valueWithScore替换现在cache中对应key的相应数据
     * 
     * @param key
     * @param valueWithScore为map类型
     *        ，其value为实际要追加的元素值，其key(须可转化为long)为相应value的score值，
     *        set中元素的排序以其score值为参考(升序排列)
     * @param sec key的过期时间，以秒为单位，必须设置过期时间，不能为永不过期，否则认为无效
     * @return 操作成功返回true，否则返回false
     * @throws TripodException
     */
    public boolean zSetWrite(String key, Map<String, String> valueWithScore, int sec) throws TripodException {
        return tripodClient.zSet(key, valueWithScore, sec);
    }

    /**
     * 有序集合追加元素接口，若当前key不在cache中(可能因为a:还未对此key设置过value值;b:该key在cache中已过期)，则返回false
     * 1.使用之前必须使用zSet初始化写接口zSetWrite来初始化
     * 2.如果调用失败，必须使用zSet初始化写接口zSetWrite来初始化来重新设置zset，否则之后的zSetAdd调用会一直失败
     * @param key
     * @param valueWithScore为map类型，其value为实际要追加的元素值，其key(须可转化为long)为相应value的score值，
     *          set中元素的排序以其score值为参考
     * @return 操作成功返回true，否则返回false
     * @throws TripodException
     */
    public boolean zSetAdd(String key, Map<String, String> valueWithScore) throws TripodException {
        if (valueWithScore != null) {
            return tripodClient.zAdd(key, valueWithScore);
        }
        return false;
    }
    
    /**
     * 有序集合删除元素接口，使用下标来删除元素
     * @param key
     * @param begin  要删除元素在set中的开始位置
     * @param limit  要删除元素的个数，若limit<0，则删除begin位置之后的所有元素
     * @return 操作成功返回true，否则返回false
     * @throws TripodException
     */
    public boolean zSetRemoveByRank(String key, int begin, int limit) throws TripodException {
        int end = 0;
        if (limit == 0) {
            return true;
        } else if (limit > 0) {
            end = begin + limit - 1;
        } else if (limit < 0) {
            end = -1;
        }
        return tripodClient.zRemRangeByRank(key, begin, end);
    }
    
    /**
     * 有序集合删除元素接口，根据元素score值删除满足条件的元素(即删除满足min=<score<=max的所有元素)
     * 元素的score值是在调用zSetWrite或zSetAdd时传入来的
     * @param key
     * @param min 要删除元素score的最小值,闭区间(min须可转化为long)
     * @param max 要删除元素score的最大值,闭区间(max须可转化为long)
     * @return
     * @throws TripodException
     */
    public boolean zSetRemoveByScore(String key, String min, String max) throws TripodException {
        return tripodClient.zRemRangeByScore(key, min, max);
    }
    
    /**
     * 有序集合获取元素接口
     * @param key
     * @param begin 起始位置，大于等于0
     * @param limit 获取元素的个数，为-1时候返回从begin开始的全部数据
     * @return 返回cache中数据，处理失败是返回null，无数据是返回空List
     * @throws TripodException
     */
    public List<String> zSetGet(String key, int begin, int limit) throws TripodException {
        return tripodClient.zRange(key, begin, limit);
    }

    /** ------------------------------------------------------- */
    /** 有序集合接口END */
    
    /** 列表接口START */
    /** ------------------------------------------------------- */
    /**
     * 列表获取接口
     * @param key
     * @param begin 要获取元素范围的起始位置，0表示列表中首元素
     * @param limit 要获取元素的个数, 为-1时候返回从begin开始到列表末尾的全部数据
     * @return 获取的元素放入列表中返回
     * @throws TripodException
     */
    public List<String> getList(String key, int begin, int limit) throws TripodException {
        return tripodClient.getListIgnoreMiss(key, begin, limit);
    }
    /**
     * 列表设置接口
     *      若连续对同一个key调用setList，最后一次调用会把前面设置的列表覆盖掉
     * @param key
     * @param value 要设置的元素列表, 若value为空返回false
     * @param secs  数据在cache中的过期时间，必须设置过期时间，不能永不过期，否则会被认为无效
     * @return  操作成功返回true
     * @throws TripodException
     */
    public boolean setList(String key, List<String> value, int secs) throws TripodException {
        if (secs <= 0) {
            return false;
        }
        return tripodClient.setList(key, value, secs);
    }

    /** ------------------------------------------------------- */
    /** 列表接口END */
}
