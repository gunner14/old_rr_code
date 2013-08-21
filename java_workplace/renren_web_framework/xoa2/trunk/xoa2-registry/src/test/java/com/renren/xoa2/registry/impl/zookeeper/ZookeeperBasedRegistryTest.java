package com.renren.xoa2.registry.impl.zookeeper;

import java.util.List;

import org.junit.Test;

import com.renren.xoa2.registry.Node;

public class ZookeeperBasedRegistryTest {	@Test
	public void test() {
		ZookeeperBasedRegistry registry = new ZookeeperBasedRegistry();
		registry.init();
		
		List<Node> nodes = registry
				.queryService("unittest.test.xoa.renren.com");
		System.out.println("Service list size : " + nodes.size());
		for (Node n : nodes) {
			System.out.println("\t" + n.getHost() + ":" + n.getPort() + " disabled=" + n.isDisabled());
		}
	}
}
