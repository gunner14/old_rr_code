package com.renren.xoa.registry.impl.zookeeper;

import org.junit.Assert;
import org.junit.Test;

public class PathUtilTest {

	@Test
	public void testToZnodePath() {
		String serviceId = "photo.ugc.xoa.renren.com";
		String path = PathUtil.toZnodePath(serviceId);
		Assert.assertEquals("/com/renren/xoa/ugc/photo", path);
	}
	
	@Test
	public void testToServiceId() {
		
		String serviceId = "photo.ugc.xoa.renren.com";
		String path = PathUtil.toZnodePath(serviceId);
		
		String serviceId2 = PathUtil.toServiceId(path);
		
		Assert.assertEquals(serviceId, serviceId2);
	}
	
}
