package xce.tripod;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public abstract class AbstractCacheClient implements CacheClient {

    public byte[] get(String key)  {
        return null;
    }

    public Map<String, byte[]> multiGet(List<String> keys) throws TripodException {
        return new HashMap<String, byte[]>();
    }

    public boolean exists(String key) throws TripodException {
        return false;
    }

    public Long size(String key) throws TripodException {
        return (long) -1;
    }

    public List<String> getList(String key, int begin, int limit) {
        return new ArrayList<String>();
    }

    public Map<String, String> getHash(String key, List<String> fields) {
        return new HashMap<String, String>();
    }

}
