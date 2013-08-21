package com.renren.xoa.registry.impl.zookeeper;

import org.junit.Assert;
import org.junit.Test;

public class ZookeeperFactoryTest {

	@Test
	public void testLoadConnectString() {
		String connectString = ZooKeeperFactory.getConnectString();
		Assert.assertNotNull(connectString);
		Assert.assertTrue(connectString.endsWith("/xoa"));
	}
	
}
