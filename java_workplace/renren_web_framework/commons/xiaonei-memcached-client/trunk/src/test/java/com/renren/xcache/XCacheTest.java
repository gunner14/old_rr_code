package com.renren.xcache;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class XCacheTest {

    @Test
    public void testFactoryClass() {
        XCache.setFactoryClass("com.renren.xcache.MockCacheFactory");
        XCache<Object> cache = XCache.getCache("asd.dasd", Object.class);
        Object v1 = new Object();
        cache.set("1", v1, 1000);
        assertTrue(v1 == cache.get("1"));
    }
}
