package com.renren.xcache.expire;

import org.junit.Test;
import static org.junit.Assert.*;


import com.renren.xcache.XCache;
import com.renren.xcache.model.User;

public class ExpireTest {

	@Test
	public void testLongTime() {
		XCache<User> cache = XCache.getCache("test.user", User.class);
		String id = "1";
		boolean succ = cache.set(id, new User(1, "haha", 12), XCache.EXPIRE_DAY * 30);
		assertTrue(succ);
		User u = cache.get(id);
		assertNotNull(u);
	}
	
	@Test
	public void testTooLongTime() {
		XCache<User> cache = XCache.getCache("test.user", User.class);
		String id = "1";
		boolean fail = false;
		try {
			cache.set(id, new User(1, "haha", 12), XCache.EXPIRE_DAY * 31);
		} catch (Exception e) {
			fail = true;
		}
		assertTrue(fail);
	}
	
}
