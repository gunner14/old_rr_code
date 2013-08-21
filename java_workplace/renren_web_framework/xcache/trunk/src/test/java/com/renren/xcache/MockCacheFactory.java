package com.renren.xcache;

public class MockCacheFactory implements XCacheFactory {

	@Override
	public <T> XCache<T> getCache(String prefix, Class<T> klass, boolean isCounter) {
		return new MockXCache<T>();
	}

}
