package com.renren.xoa.commons.spdy.datastructure;

import static org.junit.Assert.*;

import java.util.Set;
import java.util.Map.Entry;

import org.junit.Test;

/**
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-7 下午03:30:46
 */
public class TestNVBlock {

	@Test
	public void testData() {
		
		NameValueBlock nv = new NameValueBlock();
		
		nv.put("name1", "value1");
		nv.put("", "");
		nv.put("name2", "value2");
		
		byte[] bs1 = nv.toByteStructure0();
		byte[] bs2 = nv.toByteStructure();
		assertArrayEquals(bs1, bs2);
	}
	
	//@Test
	public void testPerfomance() {
		
		NameValueBlock nv = new NameValueBlock();
		
		for (int i = 0; i < 10; i++) {
			nv.put("name" + i, "value" + i);
		}
		
		int n = 2000000;
		
		long startTime1 = System.currentTimeMillis();
		for (int i = 0; i < n; i++) {
			nv.toByteStructure0();
		}
		long endTime1 = System.currentTimeMillis();
		long t1 = endTime1 - startTime1;
		
		
		long startTime2 = System.currentTimeMillis();
		for (int i = 0; i < n; i++) {
			nv.toByteStructure();
		}
		long endTime2 = System.currentTimeMillis();
		
		long t2 = endTime2 - startTime2;
		
		System.out.println("toByteStructure0 takes " + t1 + "ms");
		System.out.println("toByteStructure takes " + t2 + "ms");
		
		double per = (t1 - t2) * 1.0 / t1 * 100;
		System.out.println("enhancement:" + per + "%");
	}
	
	@Test
	public void testContructor1() {
		NameValueBlock nv = new NameValueBlock();
		nv.put("name1", "value1");
		nv.put("", "");
		nv.put("name2", "value2");
		nv.put("111111", "2222222");
		nv.put("", "");
		
		byte[] data = nv.toByteStructure();
		
		NameValueBlock nv2 = new NameValueBlock(data);
		assertEquals(nv2.size(), nv.size());
		for (Entry<String, String> entry : nv.entrySet()) {
			assertEquals(entry.getValue(), nv2.get(entry.getKey()));
		}
	}
}
