package com.renren.xoa2.registry.impl.zookeeper;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.util.List;

import org.junit.Test;

import com.renren.xoa2.registry.Node;

public class ZkBasedRegistryTest {
	@Test
	public void test() {
		ZkBasedRegistry registry = new ZkBasedRegistry();
		
		List<Node> nodes = registry
				.queryService("unittest.test.xoa.renren.com");
		System.out.println("Service list size : " + nodes.size());
		for (Node n : nodes) {
			System.out.println("\t" + n.getHost() + ":" + n.getPort() + " disabled=" + n.isDisabled());
		}
	}
	
	@Test
	public void testHostConfig() {
		ZookeeperBasedRegistry registry = new ZookeeperBasedRegistry();
		registry.init();
		
		System.setProperty("xoa2.hosts.x.y.xoa.renren.com", "127.0.0.1:8181");
		List<Node> nodes = registry.queryService("x.y.xoa.renren.com");
		assertEquals(1, nodes.size());
		assertEquals("127.0.0.1", nodes.get(0).getHost());
		assertEquals(8181, nodes.get(0).getPort());
		assertFalse(nodes.get(0).isDisabled());
		
		
		System.setProperty("xoa2.hosts.x.z.xoa.renren.com", "127.0.0.1:8181,127.0.0.2:8182");
		nodes = registry.queryService("x.z.xoa.renren.com");
		try {
			Thread.sleep(1200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		assertEquals(2, nodes.size());
		
		assertEquals("127.0.0.1", nodes.get(0).getHost());
		assertEquals(8181, nodes.get(0).getPort());
		assertFalse(nodes.get(0).isDisabled());
		
		assertEquals("127.0.0.2", nodes.get(1).getHost());
		assertEquals(8182, nodes.get(1).getPort());
		assertFalse(nodes.get(1).isDisabled());
	}
}
