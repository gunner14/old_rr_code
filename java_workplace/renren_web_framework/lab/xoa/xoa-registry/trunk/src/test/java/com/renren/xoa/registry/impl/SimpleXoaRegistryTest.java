package com.renren.xoa.registry.impl;

import junit.framework.Assert;

import org.junit.Test;

import com.renren.xoa.registry.XoaServiceDescriptor;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-14 上午11:42:40
 */
public class SimpleXoaRegistryTest {

	@Test
	public void test() {
		
		String serviceId = "test.xoa.renren.com";
		int port = 8188;
		int ipCount = 5;
		String ipPrefix = "10.3.1.";
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		
		for (int i = 1; i <= ipCount; i++) {
			reg.register(serviceId, ipPrefix + i, port);
		}
		
		for (int i = 1; i < 100; i++) {
			XoaServiceDescriptor desc = reg.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			
			int lastNumber = (i % ipCount);
			if (lastNumber == 0) {
				lastNumber = ipCount;
			}
			Assert.assertEquals(ipPrefix + lastNumber, desc.getIpAddress());
		}
	}
	
	
}
