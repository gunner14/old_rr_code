package com.renren.xcache;

import java.util.HashMap;
import java.util.Map;

public class MockXCache<T> extends XCache<T> {

    private Map<String, T> map = new HashMap<String, T>();

    @Override
    public boolean add(String id, Object value, int expireInSecond) {
        throw new UnsupportedOperationException();
    }

    @Override
    public long decr(String id, long inc) {
        throw new UnsupportedOperationException();
    }

    @Override
    public boolean delete(String id) {
        map.remove(id);
        return true;
    }
    
	@Override
	@SuppressWarnings("unchecked")
    public boolean set(String id, Object value, int expireInSecond) {
        map.put(id, (T)value);
        return true;
    }

    @Override
    public T get(String id) {
        return map.get(id);
    }

    @Override
    public long incr(String id, long inc) {
        throw new UnsupportedOperationException();
    }

	@Override
	public Map<String, T> getMulti(String[] ids) {
		throw new UnsupportedOperationException();
	}

	@Override
	public boolean isCounter() {
		return false;
	}


	@Override
	public T[] getMultiAsArray(String[] ids) {
		throw new UnsupportedOperationException();
	}

	@Override
	public StatsManager getStatsManager() {
		// TODO Auto-generated method stub
		return null;
	}
}
