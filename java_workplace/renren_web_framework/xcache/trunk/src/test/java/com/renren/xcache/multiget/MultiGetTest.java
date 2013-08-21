package com.renren.xcache.multiget;

import java.util.Map;
import java.util.Random;

import org.junit.Test;
import static org.junit.Assert.*;

import com.renren.xcache.XCache;
import com.renren.xcache.model.User;

public class MultiGetTest {

	@Test
	public void testMultiGet() {
		
		User[] users = genUsers(200);
		String[] ids = new String[users.length];
		XCache<User> cache = XCache.getCache("test.user", User.class);
		for (int i = 0; i < users.length; i++) {
			String id = "" + users[i].getId();
			ids[i] = id;
			cache.set(id, users[i], XCache.EXPIRE_SECOND * 10);
		}
		Map<String, User> map = cache.getMulti(ids);
		
		for (User u : users) {
			User u2 = map.get(u.getId() + "");
			assertEquals(u, u2);
		}
		
	}
	
	@Test
	public void testMultiGetAsArray() {
		
		User[] users = genUsers(10);
		String[] ids = new String[users.length];
		XCache<User> cache = XCache.getCache("test.user", User.class);
		for (int i = 0; i < users.length; i++) {
			String id = "" + users[i].getId();
			ids[i] = id;
			cache.set(id, users[i], XCache.EXPIRE_SECOND * 60);
		}
		User[] usersInCache = cache.getMultiAsArray(ids);
		
		for (int i = 0; i < usersInCache.length; i++) {
			assertEquals(users[i], usersInCache[i]);
		}
	}
	
	@Test
	public void testMultiGetCounter() {
		int n = 20;
		XCache<Long> cache = XCache.getCache("test.c", Long.class, true);
		String[] ids = new String[n];
		for (int i = 0; i < n; i++) {
			String id = "" + i;
			ids[i] = id;
			cache.set(id, 0l, XCache.EXPIRE_SECOND * 10);
			cache.incr(id, 1);
		}
		Map<String, Long> map = cache.getMulti(ids);
		for (int i = 0; i < ids.length; i++) {
			Long l = map.get(ids[i]);
			assertEquals(1, l.longValue());
		}
	}
	
	private User[] genUsers(int n) {
		Random ran = new Random();
		User[] users = new User[n];
		for (int i = 0; i < users.length; i++) {
			users[i] = new User(i, "n" + ran.nextInt(1000), ran.nextInt(100));
		}
		return users;
	}
	
}
