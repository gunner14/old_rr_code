package com.renren.xoa2.registry.impl.zookeeper;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.impl.NodeImpl;

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
	public void testRegisterService() {
		ZkBasedRegistry registry = new ZkBasedRegistry();
		NodeImpl node = new NodeImpl();
		node.setDisabled(false);
		node.setHost("127.0.0.1");
		node.setPort(18889);
		
		assertTrue(registry.registerService("testRegisterService.test1.xoa.renren.com", node));

		List<Node> nodes = registry.queryService("testRegisterService.test1.xoa.renren.com");
		assertEquals(node.getHost(), nodes.get(0).getHost());
		assertEquals(node.getPort(), nodes.get(0).getPort());
		assertEquals(node.isDisabled(), nodes.get(0).isDisabled());		

		registry.disableService("testRegisterService.test1.xoa.renren.com", node);
		try {
			// 等待更新生效
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		nodes = registry.queryService("testRegisterService.test1.xoa.renren.com");
		assertEquals(true, nodes.get(0).isDisabled());
	}
}
