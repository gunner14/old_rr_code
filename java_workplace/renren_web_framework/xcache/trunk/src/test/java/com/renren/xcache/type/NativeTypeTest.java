package com.renren.xcache.type;

import static org.junit.Assert.*;
import org.junit.Test;

import com.renren.xcache.XCache;

public class NativeTypeTest {

	
	@Test
	public void testByte() {
		XCache<Byte> cache = XCache.getCache("test.type-byte", Byte.class);
		String id = "1";
		Byte value = new Byte((byte)43); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Byte ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test
	public void testBoolean() {
		XCache<Boolean> cache = XCache.getCache("test.type-bool", Boolean.class);
		String id = "1";
		Boolean value = new Boolean(false); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Boolean ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testInteger() {
		XCache<Integer> cache = XCache.getCache("test.type-integer", Integer.class);
		String id = "1";
		Integer value = new Integer(123); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Integer ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testLong() {
		XCache<Long> cache = XCache.getCache("test.type-long", Long.class);
		String id = "1";
		Long value = new Long(12345678901234l); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Long ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testFloat() {
		XCache<Float> cache = XCache.getCache("test.type-float", Float.class);
		String id = "1";
		Float value = new Float(12.34); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Float ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testDouble() {
		XCache<Double> cache = XCache.getCache("test.type-double", Double.class);
		String id = "1";
		Double value = new Double(12.3456789); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Double ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testShort() {
		XCache<Short> cache = XCache.getCache("test.type-short", Short.class);
		String id = "1";
		Short value = new Short((short)125); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Short ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test 
	public void testCharacter() {
		XCache<Character> cache = XCache.getCache("test.type-char", Character.class);
		String id = "1";
		Character value = new Character('C'); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		Character ret = cache.get(id);
		assertEquals(ret, value);
	}
	
	@Test
	public void testByteArray() {
		XCache<byte[]> cache = XCache.getCache("test.type-bytea", byte[].class);
		String id = "1";
		byte[] value = "hello memcached".getBytes(); 
		boolean succ = cache.set(id, value, XCache.EXPIRE_MINUTE);
		assertTrue(succ);
		
		byte[] ret = cache.get(id);
		assertArrayEquals(ret, value);
	}
	
}
