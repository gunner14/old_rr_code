package xce.tripod;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

/**
 * 提供单表查询的通用client
 * 
 * @author lili
 * @email li.li1@renren-inc.com
 * 
 */

public class SingleTableTripodClient {

    private TripodClient tripodClient = null;

    public SingleTableTripodClient(String namespaceId, String businessId) {
        tripodClient = new TripodClient(namespaceId, businessId);
    }

    /**
     * getAndIgnoreMiss
     * 
     * @param key
     * @return protobuf序列化的byte string
     * @throws TripodException
     */
    public byte[] getAndIgnoreMiss(String key) throws TripodException {
        byte[] result = null;
        result = tripodClient.getAndIgnoreMiss(key);
        return result;
    }

    /**
     * get
     * 
     * @param key
     * @param timeOut
     * @return protobuf序列化的byte string
     * @throws TripodException
     */
    public byte[] get(String key, int secs) throws TripodException {
        byte[] result = null;
        result = tripodClient.get(key, secs);
        return result;
    }

    /**
     * 批量get
     * 
     * @param keys
     * @param timeOut
     * @return map，值为 protobuf序列化的byte string
     * @throws TripodException
     */
    public Map<String, byte[]> multiGet(List<String> keys, int secs) throws TripodException {
        Map<String, byte[]> result = null;
        // try {
        result = new HashMap<String, byte[]>();
        Map<String, byte[]> mapValue = tripodClient.multiGet(keys, secs);
        for (Map.Entry<String, byte[]> m : mapValue.entrySet()) {
            result.put(m.getKey(), m.getValue());
            // System.out.println(m.getKey());
            // System.out.println(m.getValue());
            // System.out.println(m.getValue().length);
        }

        // } catch (Exception e) {
        // e.printStackTrace();
        // }
        return result;
    }
}
