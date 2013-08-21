package com.renren.xcache.counter;

import org.junit.Test;
import static org.junit.Assert.*;
import com.renren.xcache.XCache;

public class CounterTest {

	@Test
	public void testContructor() {
		XCache<Long> counter = XCache.getCache("test.counter", Long.class, true);
		assertTrue(counter.isCounter());
		XCache<Integer> counter2 = XCache.getCache("test.counter", Integer.class, true);
		assertTrue(counter2.isCounter());
	}
	
	@Test
	public void testContructorError() {
		boolean fail = false;
		try {
			XCache.getCache("test.counter", String.class, true);
		} catch (IllegalArgumentException e) {
			fail = true;
		}
		assertTrue(fail);
	}
	
	
	@Test
	public void testIncr() {
		
		XCache<Long> counter = XCache.getCache("test.counter", Long.class, true);
		XCache<String> scounter = XCache.getCache("test.counter", String.class);
		String id = "test1";
		
		long n = 0;
		boolean succ = counter.set(id, n, XCache.EXPIRE_MINUTE * 5);
		assertTrue(succ);
		while (n++ < 200) {
			long c = counter.incr(id, 1);
			assertTrue(c == n);
			String sc = scounter.get(id);
			assertEquals(n + "", sc);
		}
		
	}
	
	@Test
	public void testDesc() {
		
		XCache<Long> counter = XCache.getCache("test.counter", Long.class, true);
		XCache<String> scounter = XCache.getCache("test.counter", String.class);
		String id = "test1";
		
		long n = 10;
		boolean succ = counter.set(id, n, XCache.EXPIRE_MINUTE * 5);
		assertTrue(succ);
		while (n-- > 0) {
			long c = counter.decr(id, 1);
			assertEquals(c, n);
			String sc = scounter.get(id).trim();	//尾部可能会有空格
			assertEquals(n + "", sc);
		}
	}
	
	@Test
	public void testAll() {
		
		XCache<Long> cache = XCache.getCache("test.counter", Long.class, true);
		String id = "5";
		long counter = cache.incr(id, 1);
		assertTrue(counter == -1);
		boolean succ = cache.set(id, 0L, XCache.EXPIRE_SECOND * 5);
		assertTrue(succ);
		counter = cache.incr(id, 1);
		assertTrue(counter == 1);
		counter = cache.incr(id, 2);
		assertTrue(counter == 3);
		
		counter = cache.decr(id, 2);
		assertTrue(counter == 1);
		
		counter = cache.decr(id, 1);
		assertTrue(counter == 0);
		
		counter = cache.get(id);
		assertTrue(counter == 0);
	}
	
}
