package com.renren.xcache;

import java.util.HashMap;
import java.util.Map;

public class MockXCache extends XCache {

    private Map<String, Object> map = new HashMap<String, Object>();

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
    public boolean set(String id, Object value, int expireInSecond) {
        map.put(id, value);
        return true;
    }

    @Override
    public Object get(String id) {
        return map.get(id);
    }

    @Override
    public long incr(String id, long inc) {
        throw new UnsupportedOperationException();
    }

	@Override
	public Map<String, Object> getMulti(String[] ids) {
		throw new UnsupportedOperationException();
	}

	@Override
	public boolean isCounter() {
		return false;
	}

}
