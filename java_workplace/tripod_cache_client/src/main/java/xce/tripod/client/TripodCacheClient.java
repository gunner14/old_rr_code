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

    public TripodCacheClient(String namespaceId, String businessId) {
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        tripodClient = new TripodClient(namespaceId, businessId);

    }

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

    /** 计数器接口BEGIN */
    /** ------------------------------------------------------- */

    /**
     * 计数器增减接口 1. 计数器只支持非负整数，成功返回正常值，异常返回ERROR_COUNT 2.
     * 在调用之前，需要用[计数器初始化接口]来初始化指定过期时间，否则会自动生成一个初值为0且永不过期的值
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

    /**
     * 计数器初始化接口
     * 
     * @param key
     * @param value 初始值
     * @param expireTime 过期时间
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

    /** ------------------------------------------------------- */
    /** 计数器接口END */

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

    /**
     * binary写 接口
     * 
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
     * 
     * @param key
     * @return
     * @throws TripodException
     */
    public byte[] binaryGet(String key) throws TripodException {
        return tripodClient.getAndIgnoreMiss(key);
    }

    /**
     * binary批量取接口
     * 
     * @param keyList
     * @return
     * @throws TripodException
     */
    public Map<String, byte[]> binaryMultiGet(List<String> keyList) throws TripodException {
        return tripodClient.multiGetAndIngoreMiss(keyList);
    }

}
