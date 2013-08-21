package com.renren.xcache.pool;

import com.renren.xcache.XCache;

public class DataSetter {

	public static void main(String[] args) {
		XCache<Integer> cache = XCache.getCache("test.test", Integer.class);
		for (int id = 0; id < 1; id++) {
			cache.set(String.valueOf(id), id, 3600);
		}
		System.out.println("done");
	}
	
}
