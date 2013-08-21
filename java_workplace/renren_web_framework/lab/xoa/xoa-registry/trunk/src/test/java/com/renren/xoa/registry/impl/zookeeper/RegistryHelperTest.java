package com.renren.xoa.registry.impl.zookeeper;

import java.util.List;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;

public class RegistryHelperTest {
	
	private static final String serviceId = "test.xoa.renren.com";
	
	private static RegistryHelper helper = new RegistryHelper();
	
	@BeforeClass
	public static void reset() {
		
		//重置测试状态
		ZookeeperBasedRegistry reg = new ZookeeperBasedRegistry();
		reg.init();
		
		XoaService service = reg.getService(serviceId);
		if (service != null) {
			List<XoaServiceDescriptor> nodes = service.getNodes();
			if (nodes != null && nodes.size() > 0) {
				for (XoaServiceDescriptor desc : nodes) {
				    helper.deleteServiceNode(desc);
				}
			}
		}
	}
	
	@AfterClass
	public static void destroy() {
	    helper.destroy();
	    helper = null;
	}
	
	@Test
	public void testToConfigFilePath() {
		
		String serviceId = "test.xoa.renren.com";
		String ip = "10.3.1.1";
		int port = 8188;
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId(serviceId);
		desc.setIpAddress(ip);
		desc.setPort(port);
		
		String path = RegistryHelper.toConfigFilePath(desc);
		
		Assert.assertEquals(
				"/com/renren/xoa/test/.service-nodes/10.3.1.1:8188", path);

	}
	
	@Test
	public void testRegisterNode() {
		
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId(serviceId);
		desc.setIpAddress("10.3.1.1");
		desc.setPort(8188);
		
		boolean succ = helper.registerServiceNode(desc);
		Assert.assertTrue(succ);
	}
	
	@Test
	public void testUpdateNode() {
		
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId(serviceId);
		desc.setIpAddress("10.3.1.1");
		desc.setPort(8188);
		desc.setDisabled(true);
		boolean succ = helper.updateServiceNode(desc);
		Assert.assertTrue(succ);
	}
	
	@Test
	public void testDeleteNode() {
		
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId(serviceId);
		desc.setIpAddress("10.3.1.1");
		desc.setPort(8188);
		
		boolean succ = helper.deleteServiceNode(desc);
		Assert.assertTrue(succ);
	}
	
	
	
}
