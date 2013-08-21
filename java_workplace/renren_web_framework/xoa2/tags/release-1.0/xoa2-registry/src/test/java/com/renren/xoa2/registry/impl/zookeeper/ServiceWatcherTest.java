package com.renren.xoa2.registry.impl.zookeeper;

import static org.junit.Assert.assertEquals;

import java.io.IOException;

import org.apache.zookeeper.ZooKeeper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.List;

import com.renren.xoa2.registry.ServiceGroup;
import com.renren.xoa2.registry.Shard;
import com.renren.xoa2.registry.Node;

public class ServiceWatcherTest {
	final private static String ZK_ADDR = "10.11.21.185:2181,10.11.21.186:2181,10.3.18.186:2181/xoa2";
	private static ZooKeeper zookeeper = null;
	
	@Before
	public void setUp() throws InterruptedException, IOException {
		zookeeper = new ZooKeeper(ZK_ADDR,	3000000, null);		
	}
	@After
	public void tearDown() throws Exception {
       zookeeper.close();
	}
	

	@Test
	public void notExistServiceTest() {
		ServiceWatcher service = new ServiceWatcher("/com/renren/xoa/test/notexist");
		assertEquals(false, service.load());
	}

	@Test
	public void goodServiceTest() {
		ServiceWatcher service = new ServiceWatcher("/com/renren/xoa/test/unittest");
		assertEquals(true, service.load());
		assertEquals("unittest.test.xoa.renren.com", service.getId());
		ServiceGroup serviceGroup = service.getServiceGroup();

		assertEquals(0, serviceGroup.getId());
		assertEquals(1, serviceGroup.getShardFactor());
		assertEquals(0, serviceGroup.getPriority());
	    
	    Shard shard = serviceGroup.getShard(100);
		assertEquals(0, shard.getId());
		List<Node> nodes = shard.getNodes();
		assertEquals(6, nodes.size());
	}

	@Test
	public void childEventTest() {
		ServiceWatcher service = new ServiceWatcher("/com/renren/xoa/test/unittest");
		assertEquals(true, service.load());		
		
		for (int i = 100; i < 110; ++i) {
			PathUtil.createNode(zookeeper, 
			        "/com/renren/xoa/test/unittest/.service_group_" + i, 
			        (new String("priority=" + i)).getBytes(), true);

			System.out.println("round : " + i);
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			ServiceGroup serviceGroup = service.getServiceGroup();
			assertEquals(i, serviceGroup.getId());
			assertEquals(i, serviceGroup.getPriority());

			PathUtil.removeZode(zookeeper, "/com/renren/xoa/test/unittest/.service_group_" + i);

			System.out.println("round : " + i);
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

		}
	}
}
