package com.renren.xcache.type;

import org.junit.Test;
import static org.junit.Assert.*;

import com.renren.xcache.XCache;

public class TypeCheckTest {

	@SuppressWarnings("unchecked")
	@Test
	public void testSet() {
		
		XCache cache = XCache.getCache("test.type-t", Integer.class);
		boolean flag = false;
		try {
			cache.set("111", "haha", XCache.EXPIRE_MINUTE);
		} catch (IllegalArgumentException e) {
			flag = true;
		}
		assertTrue(flag);
	}
	
	@Test
	public void testSetSubclass1() {
		
		XCache<Object> cache = XCache.getCache("test.type-sub");
		boolean flag = false;
		try {
			cache.set("111", "haha", XCache.EXPIRE_MINUTE);
		} catch (IllegalArgumentException e) {
			flag = true;
		}
		assertFalse(flag);
	}
	
	@SuppressWarnings("unchecked")
	@Test
	public void testSetSubclass2() {
		
		XCache cache = XCache.getCache("test.type-sub", Number.class);
		boolean flag = false;
		try {
			cache.set("111", "haha", XCache.EXPIRE_MINUTE);
		} catch (IllegalArgumentException e) {
			flag = true;
		}
		assertTrue(flag);
		flag = false;
		try {
			cache.set("111", new Double(2.0), XCache.EXPIRE_MINUTE);
		} catch (IllegalArgumentException e) {
			flag = true;
		}
		assertFalse(flag);
	}
	
	@Test
	public void testGet() {
		
		String id = "111";
		int[] value = new int[]{1,2};
		XCache<int[]> cache = XCache.getCache("test.type-t", int[].class);
		cache.set(id, value, XCache.EXPIRE_SECOND * 10);
		
		int[] ret = cache.get(id);
		assertArrayEquals(ret, value);
		
		boolean flag = false;
		XCache<String> cache2 = XCache.getCache("test.type-t", String.class);
		try {
			cache2.get(id);
		} catch (ClassCastException e) {
			flag = true;
		}
		assertTrue(flag);
		
	}
	
}
