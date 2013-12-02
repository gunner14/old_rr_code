package xce.tripod.checker;

import xce.tripod.CacheClient;
import xce.tripod.CacheManagerProxy;
import xce.tripod.CacheProxy;
import xce.tripod.ProducerManagerProxy;
import xce.tripod.TripodException;

/**
 * <p>
 * Title: TripodCommonChecker.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-5-31
 */
public class UserNetworkTripodChecker extends AbstractChecker {

    private String namespace = "";

    private String bizId = "";

    private CacheClient mCacheClient = null;

    private ProducerManagerProxy mProducerProxy = null;

    private CacheManagerProxy mCacheManagerProxy = null;

    public UserNetworkTripodChecker(String namespace, String bizId) {
        this.namespace = namespace;
        this.bizId = bizId;
        mCacheClient = new CacheProxy(namespace, bizId, 1000);
        mProducerProxy = new ProducerManagerProxy(namespace, bizId);
        mCacheManagerProxy = new CacheManagerProxy(namespace, bizId, 1000);
    }

    /*
     * (non-Javadoc)
     * 
     * @see mop.hi.oce.adapter.impl.Checker#compare(java.lang.Object,
     * java.lang.Object)
     */
    @Override
    protected boolean compare(Object dataFromCache, Object dataFromDB) {
        if (dataFromCache == null) {
            log("Cache data is null!");
            return false;
        }

        if (dataFromDB == null) {
            log("DB data is null!");
            return false;
        }

        byte[] dataA = (byte[]) dataFromCache;
        byte[] dataB = (byte[]) dataFromDB;

        if (dataA.length != dataB.length) {
            log("the size of cache data is not the same as the size of DB data");
            return false;
        }

        for (int i = 0; i < dataA.length; ++i) {
            if (dataA[i] != dataB[i]) {
                log("cache data content is different from DB data content");
                return false;
            }
        }

        return true;
    }

    /*
     * (non-Javadoc)
     * 
     * @see mop.hi.oce.adapter.impl.Checker#reloadData(int)
     */
    @Override
    protected boolean reloadData(int id) {
        log("reload data id " + id);
        try {
            boolean result = mCacheManagerProxy.load(String.valueOf(id), 30 * 24 * 3600, true);
            return result;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see mop.hi.oce.adapter.impl.Checker#getDataFromDB(int)
     */
    @Override
    protected Object getDataFromDB(int id) {
        return mProducerProxy.produce(String.valueOf(id), true);

    }

    /*
     * (non-Javadoc)
     * 
     * @see mop.hi.oce.adapter.impl.Checker#getDataFromCache(int)
     */
    @Override
    protected Object getDataFromCache(int id) throws TripodException {
        try {
            return mCacheClient.get(String.valueOf(id));
        } catch (TripodException e) {
            throw e;
        }
    }

    public static void main(String[] args) {
        if (args.length < 5) {
            log("Usage: TripodCommonChecker idFile interval(ms) retryTimes");
            System.exit(0);
        }

        String namespace = args[0];
        String bizId = args[1];
        String idFile = args[2];
        int interval = Integer.parseInt(args[3]); // ms
        int retryTimes = Integer.parseInt(args[4]);

        UserNetworkTripodChecker checker = new UserNetworkTripodChecker(namespace, bizId);
        checker.startCheck(idFile, interval, retryTimes);

        System.exit(0);
    }
}
