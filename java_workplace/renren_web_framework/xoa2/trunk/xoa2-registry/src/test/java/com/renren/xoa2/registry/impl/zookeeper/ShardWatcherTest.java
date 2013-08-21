package com.renren.xoa2.registry.impl.zookeeper;


import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.io.IOException;
import java.util.HashSet;

import org.apache.zookeeper.ZooKeeper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.List;
import com.renren.xoa2.registry.Node;

public class ShardWatcherTest {
	final private static String ZK_ADDR = "10.11.21.185:2181,10.11.21.186:2181,10.3.18.186:2181/xoa2";
	private static ZooKeeper zookeeper = null;
	
	@Before
	public void setUp() throws Exception {
		try {
			zookeeper = new ZooKeeper(ZK_ADDR, 30000, null);
		} catch (IOException e) {
			throw new RuntimeException("Error occurs while creating ZooKeeper instance.", e);
		}
	}

	@After
	public void tearDown() throws Exception {
		zookeeper.close();
	}
	
	@Test
	public void testIllegalShard() {
		ShardWatcher shard = new ShardWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0",
				"shard_x");
		assertEquals(false, shard.load());
	}

	@Test
	public void testLegalShard() {
		ShardWatcher shard = new ShardWatcher(zookeeper, 
				"/com/renren/xoa/test/unittest/.service_group_0",
				"shard_0");
		assertEquals(true, shard.load());
		assertEquals(0, shard.getId());
		List<Node> nodes = shard.getNodes();
		assertEquals(6, nodes.size());

		HashSet<Integer> disabledNodes = new HashSet<Integer>();
		HashSet<Integer> enabledNodes = new HashSet<Integer>();
		for(Node n : nodes) {
			if (n.isDisabled()) {
				disabledNodes.add(n.getPort());
			} else {
				enabledNodes.add(n.getPort());				
			}
		}
		assertTrue(disabledNodes.contains(10000));
		assertTrue(disabledNodes.contains(10002));
		assertTrue(disabledNodes.contains(10003));

		assertFalse(disabledNodes.contains(10001));
		assertFalse(disabledNodes.contains(10004));
		assertFalse(disabledNodes.contains(10005));
	}
	

	@Test
	public void childChangedTest() {
		String path = "/com/renren/xoa/test/unittest/.service_group_0";
		String name = "shard_0";
		
		ShardWatcher shard = new ShardWatcher(zookeeper, path, name);
		assertTrue(shard.load());

		String tmpNode = path + '/' + name + '/' + "127.0.0.1:20001";
		String tmpNode2 = path + '/' + name + '/' + "127.0.0.1:20002";
		int size = shard.getNodes().size();
		PathUtil.createTempZode(zookeeper, tmpNode, "status=enabled".getBytes());	
		PathUtil.createTempZode(zookeeper, tmpNode2, "status=disabled".getBytes());		
		
		try {			
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(size + 2, shard.getNodes().size());

		PathUtil.removeZode(zookeeper, tmpNode);
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(size + 1, shard.getNodes().size());	
		
		PathUtil.removeZode(zookeeper, tmpNode2);	
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(size, shard.getNodes().size());	
	}
}
