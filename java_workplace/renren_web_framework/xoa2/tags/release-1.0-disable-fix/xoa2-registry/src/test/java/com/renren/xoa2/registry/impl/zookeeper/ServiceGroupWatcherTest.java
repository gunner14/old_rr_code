package com.renren.xoa2.registry.impl.zookeeper;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.io.IOException;

import org.apache.zookeeper.ZooKeeper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.renren.xoa2.registry.Shard;

public class ServiceGroupWatcherTest {
	final private static String ZK_ADDR = "10.11.21.185:2181,10.11.21.186:2181,10.3.18.186:2181/xoa2";
	private static ZooKeeper zookeeper = null;
	
	@Before
	public void setUp() throws InterruptedException, IOException {
        if (zookeeper != null && zookeeper.getState().isAlive()) {
        	zookeeper.close();
        }

		zookeeper = new ZooKeeper(ZK_ADDR,	300000, null);
		
		while (!zookeeper.getState().isAlive()) {
			System.out.println("waiting ... ");
			Thread.sleep(1);
		}
	}
	@After
	public void tearDown() throws Exception {
        if (zookeeper != null && zookeeper.getState().isAlive()) {
        	zookeeper.close();
        }
	}

	@Test
	public void noNodeTest() {
		ServiceGroupWatcher serviceGroup = 
			new ServiceGroupWatcher(zookeeper, "/com/renren/xoa/test/unittest", ".service_group_1");		
		assertEquals(false, serviceGroup.load());
	}

	@Test
	public void badNodeTest() {
		ServiceGroupWatcher serviceGroup = 
			new ServiceGroupWatcher(zookeeper, "/com/renren/xoa/test/unittest", ".service_group_x");		
		assertEquals(false, serviceGroup.load());
	}

	@Test
	public void goodNodeTest() {
		ServiceGroupWatcher serviceGroup = 
			new ServiceGroupWatcher(zookeeper, "/com/renren/xoa/test/unittest", ".service_group_0");		
		assertEquals(true, serviceGroup.load());
		assertEquals(0, serviceGroup.getId());
		assertEquals(1, serviceGroup.getShardFactor());
		assertEquals(0, serviceGroup.getPriority());
	    
	    Shard shard = serviceGroup.getShard(100);
		assertEquals(0, shard.getId());
	
	}

	@Test
	public void dataChangedTest() {
		String path = "/com/renren/xoa/test/unittest/.service_group_1000";
		PathUtil.createNode(zookeeper, path, "priority=100\r\nshard_factor=2".getBytes(), true);		
		ServiceGroupWatcher serviceGroup = 
			new ServiceGroupWatcher(zookeeper, "/com/renren/xoa/test/unittest", ".service_group_1000");
		assertTrue(serviceGroup.load());
		assertEquals(100, serviceGroup.getPriority());
		assertEquals(2, serviceGroup.getShardFactor());

		PathUtil.setZodeData(zookeeper, path, "priority=33\r\nshard_factor=5".getBytes());	
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(33, serviceGroup.getPriority());
		assertEquals(5, serviceGroup.getShardFactor());
	}
}
