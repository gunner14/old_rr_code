package xce.tripod;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;
import java.util.concurrent.locks.ReentrantReadWriteLock.WriteLock;

import com.xiaonei.xce.log.Oce;

public class MultiBizTripodClient extends TripodClient {

    public MultiBizTripodClient(String ns) {
        super(ns, "");
        multiBizCacheProxy = new MultiBizCacheProxy(ns);
        cacheManagerProxyMap = new HashMap<String, CacheManagerProxy>();
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

        CacheManagerProxy tempCacheManagerProxy = new CacheManagerProxy(namespaceId, biz);
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
        RemoteLogger.getInstance().log(dump("multiBizGet"));
        Map<String, byte[]> result = new HashMap<String, byte[]>();
        result = multiBizCacheProxy.multiBizGet(key, bizs);
        // System.out.println("***redis result***");
        // System.out.println(result);
        for (Map.Entry<String, byte[]> m : result.entrySet()) {
            if (m.getValue() == null || m.getValue().length == 0) {
                String tempBiz = m.getKey();
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
        // System.out.println("***final result***");
        // System.out.println(result);
        return result;
    }

    public Map<String, byte[]> multiBizGet(String key, List<String> bizs, int secs)
            throws TripodException {
        return multiBizGet(key, bizs, secs, false);
    }
}
