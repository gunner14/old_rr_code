package xce.tripod.client;

import java.util.HashMap;

import xce.tripod.TripodException;

/**
 * TripodCacheClientFactory 提供给网站技术同学使用
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class TripodCacheClientFactory {

    private static HashMap<Entry, TripodCacheClient> tripodCacheClientMap = new HashMap<Entry, TripodCacheClient>();

    private static Object gate = new Object();

    public static TripodCacheClient getClient(Entry entry) {
        synchronized (gate) {
            TripodCacheClient client = tripodCacheClientMap.get(entry);
            if (client == null) {
                client = new TripodCacheClient(entry.getNamespaceId(), entry.getBusinessId());
                if (client != null) {
                    tripodCacheClientMap.put(entry, client);
                }
            }
            return client;
        }

    }

    /**
     * public static void main(String[] args) { TripodCacheClient client =
     * TripodCacheClientFactory.getClient(EntryType.BIZ_PAGE_DEFAULT);
     * String key = "hellokey"; String value = "hellovalue"; int expireTime
     * = 1000; try { client.set(key, value, expireTime);
     * System.out.println(client.get(key)); } catch (TripodException e) {
     * e.printStackTrace(); }
     * 
     * System.exit(0); }
     */
}
