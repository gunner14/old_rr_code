package com.xiaonei.platform.core.cache;

import static org.junit.Assert.*;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import net.spy.memcached.MemcachedClient;

import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Level;
import org.apache.log4j.LogManager;
import org.junit.Before;
import org.junit.Test;

public class TestCache {
	MemCacheManager manager;
	@Before
	public void init(){
		System.setProperty("net.spy.log.LoggerImpl", "net.spy.memcached.compat.log.Log4JLogger");
		LogManager.getRootLogger().setLevel(Level.OFF);
		//LogManager.getLogger("net.spy.memcached").setLevel(Level.INFO);
		BasicConfigurator.configure();
		manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
		assertTrue(manager!=null);
	}
	
	@Test
	public void testYear(){
		//long year = MemCacheKeys.year;
		System.out.println(MemCacheKeys.year);
	}
	
	@Test
	public void testRoundRobin(){
		assertTrue(manager.getValidClientCount()==4);
		assertTrue(manager.getCurrentNext()==0);
		manager.get("null");
		assertTrue(manager.getCurrentNext()==1);
		
	}
	@Test
	public void testManager(){
		MemCacheManager manager = new MemCacheManager("default");
		manager.set("name","zhangjie",0);
		Object value = manager.get("name");
		assertTrue(value.toString().equals("zhangjie"));
		
		manager.update("");
		
		try {
			Thread.sleep(6000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		manager.update("");
		System.out.println("OVER");
	}
	@Test
	public void testFactory(){
		MemCacheManager manager = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
		assertTrue(manager!=null);
		
		MemCacheFactory.getInstance().checkPoolInit();
		try {
			Thread.sleep(1000000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	@Test
	public void testShutDown(){
		manager.shutdown();
		try {
			Thread.sleep(10000000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	@Test
	public void testGetHost(){
		assertTrue(manager!=null);
		System.out.println(manager.getHost("name"));//19.3
		System.out.println(manager.getHost("key"));//19.3
		System.out.println(manager.getHost("gg"));//19.2
	}
	@Test
	public void testCacheObject(){
		List<String> list = new ArrayList<String>();
		list.add("zhang");list.add("jie");
		
		assertTrue(manager!=null);
		manager.set("list", list);
		
		List<String> c = (List<String>)manager.get("list");
		assertTrue(c.get(0).equals("zhang"));
		assertTrue(c.get(1).equals("jie"));
	}
	@Test
	public void testGet(){
		manager.set("name", "zhangjie");
		assertTrue(manager.get("name").toString().equals("zhangjie"));
		Future<Object> f = manager.asyncGet("name");
		try{
			Object value = f.get(1, TimeUnit.SECONDS);
			assertTrue(value.toString().equals("zhangjie"));
			System.out.println(value);
		}catch(Exception ex){
			ex.printStackTrace();
			assertTrue(true);
			System.out.println("timeout");
		}
		TestSer s = new TestSer("zhangjie",27);
		manager.set("test", s);
		TestSer t = (TestSer)manager.get("test");
		assertTrue(t.name.equals("zhangjie"));
		assertTrue(t.sex == 27);
	}
	@Test
	public void testGetBulk(){
		manager.set("key1", "key1");
		manager.set("key2", "key2");
		manager.set("key3", "key3");
		
		Map<String,Object> m = manager.getBulk("key1","key2","key3");
		assertTrue(m!=null);
		assertTrue(m.size()==3);
		assertTrue(m.get("key1").equals("key1"));
		assertTrue(m.get("key2").equals("key2"));
		assertTrue(m.get("key3").equals("key3"));
		
		Future<Map<String,Object>> f = manager.asyncGetBulk("key1","key2","key3");
		try{
			Map<String,Object> fm = f.get(50,TimeUnit.MILLISECONDS);
			assertTrue(fm.size()==3);
		}catch(Exception ex){
			ex.printStackTrace();
			assertTrue(false);
		}
	}
	@Test
	public void testIncr() throws IOException{
		MemcachedClient client = new MemcachedClient(new InetSocketAddress("10.3.17.51",11211));
		long count = client.decr("count",10);
		if(count==-1){
			client.add("count", 0, 0);
		}
	
		System.out.println(client.decr("count",0));
		System.out.println("count="+count);
		
	}
	@Test
	public void testDecr(){
		manager.decr("count");
	}
}
