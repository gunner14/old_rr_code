package xce.tripod;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import com.xiaonei.xce.log.Oce;

public class MultiBizTripodClient extends TripodClient {

    private int writeTimeOutInMilliseconds;

    public MultiBizTripodClient(String ns) {
        this(ns, 1000, 1000);
    }

    public MultiBizTripodClient(String ns, int readTimeOutInMilliseconds,
            int writeTimeOutInMilliseconds) {
        super(ns, "");
        multiBizCacheProxy = new MultiBizCacheProxy(ns, readTimeOutInMilliseconds);
        cacheManagerProxyMap = new HashMap<String, CacheManagerProxy>();
        this.writeTimeOutInMilliseconds = writeTimeOutInMilliseconds;
    }

    private ReentrantReadWriteLock rwLock = new ReentrantReadWriteLock();

    private MultiBizCacheProxy multiBizCacheProxy;

    private Map<String, CacheManagerProxy> cacheManagerProxyMap = new HashMap<String, CacheManagerProxy>();

    private CacheManagerProxy loacateCacheManagerProxy(String biz) {
        ReadLock rLock = rwLock.readLock();
        try {
            rLock.lock();
            CacheManagerProxy result = cacheManagerProxyMap.get(biz);
            if (result != null) {
                return result;
            }
        } finally {
            rLock.unlock();
        }

        CacheManagerProxy tempCacheManagerProxy = new CacheManagerProxy(namespaceId, biz,
                writeTimeOutInMilliseconds);
        WriteLock wLock = rwLock.writeLock();
        try {
            wLock.lock();
            cacheManagerProxyMap.put(biz, tempCacheManagerProxy);
        } finally {
            wLock.unlock();
        }
        return tempCacheManagerProxy;

    }

    public Map<String, byte[]> multiBizGet(String key, List<String> bizs, int secs,
            boolean useMaster) throws TripodException {
        for (String tempBiz : bizs) {
            InvokeClient.getInstance().log("multiBizGet", namespaceId, tempBiz);
        }
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        try {
            result = multiBizCacheProxy.multiBizGet(key, bizs);
        } catch (TripodException e) {
            Oce.getLogger().error(this.getClass().getName() + ".multiBizGet " //NL
                    + key + " TripodException " + e.toString());
        }
        for (String tempBiz : bizs) {
            byte[] value = result.get(tempBiz);
            if (value == null || value.length == 0) {
                InvokeClient.getInstance().log("miss_multiBizGet", namespaceId, tempBiz);
                CacheManagerProxy tempCacheManagerProxy = loacateCacheManagerProxy(tempBiz);
                try {
                    result.put(tempBiz, tempCacheManagerProxy.get(key, secs, useMaster));
                } catch (Ice.TimeoutException e) {
                    Oce.getLogger().error(
                            this.getClass().getName() + " .cacheManagerProxy get  " + tempBiz
                                    + "key" + key);
                }
            }
        }
        return result;
    }

    public Map<String, byte[]> multiBizGet(String key, List<String> bizs, int secs)
            throws TripodException {
        return multiBizGet(key, bizs, secs, false);
    }

}
