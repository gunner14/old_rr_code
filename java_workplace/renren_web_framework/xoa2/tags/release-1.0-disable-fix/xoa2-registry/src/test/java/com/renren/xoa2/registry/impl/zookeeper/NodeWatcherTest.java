package com.renren.xoa2.registry.impl.zookeeper;


import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.io.IOException;

import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class NodeWatcherTest {
	final private static String ZK_ADDR = "10.11.21.185:2181,10.11.21.186:2181,10.3.18.186:2181/xoa2";
	private static ZooKeeper zookeeper = null;
	
	@Before
	public void setUp() throws InterruptedException, IOException {
        if (zookeeper != null && zookeeper.getState().isAlive()) {
        	zookeeper.close();
        }

		// zookeeper = new ZooKeeper(ZK_ADDR,	300000, new MyWatcher());
		zookeeper = new ZooKeeper(ZK_ADDR,	300000, null);
		
		while (!zookeeper.getState().isAlive()) {
			System.out.println("waiting ... ");
			Thread.sleep(1);
		}
	}
	
    static class MyWatcher implements Watcher {
        public void process(WatchedEvent event) {
        	System.out.println("zk event --- : " + event);
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
		NodeWatcher no_exist_node = new NodeWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0/shard_0",
				"10.10.10.11:1000");
		assertEquals(false, no_exist_node.load());
	}
	
	@Test
	public void test() {
		NodeWatcher disabled_node = new NodeWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0/shard_0",
				"127.0.0.1:10002");
		assertEquals(true, disabled_node.load());
		assertEquals("127.0.0.1", disabled_node.getHost());
		assertEquals(10002, disabled_node.getPort());
		assertEquals(true, disabled_node.isDisabled());

		NodeWatcher enabled_node = new NodeWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0/shard_0",
				"127.0.0.1:10001");
		assertEquals(true, enabled_node.load());
		assertEquals("127.0.0.1", enabled_node.getHost());
		assertEquals(10001, enabled_node.getPort());
		assertEquals(false, enabled_node.isDisabled());
	}
	
	@Test
	public void badDataTest() {		
		NodeWatcher bad_data_node = new NodeWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0/shard_0",
				"127.0.0.1:10000");
		assertEquals(true, bad_data_node.load());
		assertEquals(true, bad_data_node.isDisabled());
	}
	
	@Test
	public void dataChangedTest() {
		String path = "/com/renren/xoa/test/unittest/.service_group_0/shard_0";
		String name = "127.0.0.1:20001";
		
		PathUtil.createNode(zookeeper, path + '/' + name, 
		        "status=enabled".getBytes(), true);
		NodeWatcher node = 
			new NodeWatcher(zookeeper, path, name);
		assertTrue(node.load());
		
		assertFalse(node.isDisabled());
		
		PathUtil.setZodeData(zookeeper, path + '/' + name, "status=disabled".getBytes());
		
		try {			
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		assertTrue(node.isDisabled());
	}
}
