package com.renren.xoa.registry.impl.zookeeper;

import java.util.List;

import com.renren.xoa.registry.XoaServiceDescriptor;

public class ZookeeperBasedRegistryTest {
 
	
	public static void main(String[] args) {
		
		ZooKeeperFactory.HOSTS = "10.22.200.140:2181";
		String serviceId = "test.xoa.renren.com";
		ZookeeperBasedRegistry reg = new ZookeeperBasedRegistry();
		reg.init();
		
		List<XoaServiceDescriptor> descs = reg.getServiceNodes(serviceId);
		for (XoaServiceDescriptor desc : descs) {
			System.out.println(desc);
		}
		Object flag = new Object();
		synchronized (flag) {
			try {
				flag.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
	}
	
}
