package com.xiaonei.prometheus.test;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map;

import org.junit.Test;

import tokyocabinet.TDB;

import com.xiaonei.prometheus.storage.cabinet.CabinetStore;

public class CabinetStoreTest {
	@Test
	public void test(){
		CabinetStore store = new CabinetStore("/home/wenwen/prometheus.tdb");
		Map<String,String> user = new HashMap<String,String>();
		user.put("name", "zhangjie");
		user.put("age", "27");
		user.put("blog", "http://zhangjie.renren.com");
		store.put("zhangjie", user);
		
		Map<String,String> user1 = store.get("zhangjie");
		assertTrue(user1.get("name").equals("zhangjie"));
		assertTrue(user1.get("age").equals("27"));
		assertTrue(user1.get("blog").equals("http://zhangjie.renren.com"));
	}
	@Test
	public void testQuery(){
		CabinetStore store = new CabinetStore("/home/wenwen/prometheus.tdb");
		TDB db = store.getTDB();
	}
	@Test
	public void test1(){
		TDB db = new TDB();
		db.open("/home/wenwen/workspace/Prometheus/bin/test.tdb",TDB.OREADER);
		System.out.println("open");
		db.iterinit();
		String key = db.iternext2();
		while(key!=null){
			System.out.println(key);
			key = db.iternext2();
		}
	}
}
