package com.renren.xoa.registry.impl;

import java.util.ArrayList;
import java.util.List;

import junit.framework.Assert;

import org.junit.Test;

import com.renren.xoa.registry.XoaRegistryListener;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-28 下午12:33:08
 */
public class AbstractXoaRegistryTest {
	
	@Test
	public void testQueryService() {
		
		MockRegistry registry = new MockRegistry();
		
		String serviceId = "test.xoa.renren.com";
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
		}
		
		serviceId = "test2.xoa.renren.com";
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
		}
		
		serviceId = "test3.xoa.renren.com";
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
		}
		
	}
	
	@Test
	public void testUpdateAndQeuryService() {
		
		MockRegistry registry = new MockRegistry();
		
		String serviceId = "test.xoa.renren.com";
		
		//预先query一遍
		List<XoaServiceDescriptor> nodes = new ArrayList<XoaServiceDescriptor>();
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
			
			nodes.add(desc);
		}
		
		//disable掉一个node再query一次
		int disabledIndex1 = 2;
		((XoaServiceDescriptorBase)nodes.get(disabledIndex1)).setDisabled(true);
		registry.updateServiceNodes(serviceId, nodes);
		
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			
			if (i-1 == disabledIndex1) {	//跳过disable的节点来检测
				continue;
			}
			
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
		}
		
		//再disable掉一个node再query一次
		int disabledIndex2 = 3;
		((XoaServiceDescriptorBase)nodes.get(disabledIndex2)).setDisabled(true);
		registry.updateServiceNodes(serviceId, nodes);
		for (int i = 1; i <= MockRegistry.IP_MAX; i++) {
			
			if (i-1 == disabledIndex1 || i-1 == disabledIndex2) {	//跳过disable的节点来检测
				continue;
			}
			
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals(MockRegistry.PORT, desc.getPort());
			Assert.assertEquals(MockRegistry.IP_PREFIX + i, desc.getIpAddress());
		}
	}
	
	@Test
	public void testSystemPropertyConfig() {
		
		MockRegistry registry = new MockRegistry();
		String serviceId = "test.xoa.renren.com";
		
		//设置系统属性覆盖registry配置
		System.setProperty("xoa.hosts." + serviceId, "10.2.2.0:8888,10.2.2.1:8888");
		
		//query测试
		for (int i = 0; i < 2; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals("10.2.2." + i, desc.getIpAddress());
			Assert.assertEquals(8888, desc.getPort());
		}

		//update节点设置
		registry.updateServiceNodes(serviceId, registry.getServiceNodes(serviceId));
		//再query测试一次
		for (int i = 0; i < 2; i++) {
			XoaServiceDescriptor desc = registry.queryService(serviceId);
			Assert.assertNotNull(desc);
			Assert.assertEquals(serviceId, desc.getServiceId());
			Assert.assertEquals("10.2.2." + i, desc.getIpAddress());
			Assert.assertEquals(8888, desc.getPort());
		}
	} 
	
	@Test 
	public void testInitialDisabledNodes() {
	    
	    String serviceId1 = "test1.xoa.renren.com";
	    SimpleXoaRegistry reg = new SimpleXoaRegistry();
	    reg.register(serviceId1, "10.3.1.1", 8188);
	    reg.register(serviceId1, "10.3.1.2", 8188, true);
	    reg.register(serviceId1, "10.3.1.3", 8188);
	    
	    Assert.assertEquals("10.3.1.1", reg.queryService(serviceId1).getIpAddress());
	    Assert.assertEquals("10.3.1.3", reg.queryService(serviceId1).getIpAddress());
	    Assert.assertEquals("10.3.1.1", reg.queryService(serviceId1).getIpAddress());
        Assert.assertEquals("10.3.1.3", reg.queryService(serviceId1).getIpAddress());
        
        String serviceId2 = "test2.xoa.renren.com";
        reg.register(serviceId2, "10.3.1.1", 8188);
        reg.register(serviceId2, "10.3.1.2", 8188);
        reg.register(serviceId2, "10.3.1.3", 8188, true);
        Assert.assertEquals("10.3.1.1", reg.queryService(serviceId2).getIpAddress());
        Assert.assertEquals("10.3.1.2", reg.queryService(serviceId2).getIpAddress());
        Assert.assertEquals("10.3.1.1", reg.queryService(serviceId2).getIpAddress());
        Assert.assertEquals("10.3.1.2", reg.queryService(serviceId2).getIpAddress());
	}
	
	@Test
	public void testLookup() {
	    
	    String serviceId1 = "test1.xoa.renren.com";
        SimpleXoaRegistry reg = new SimpleXoaRegistry();
        reg.register(serviceId1, "10.3.1.1", 8188);
        reg.register(serviceId1, "10.3.1.2", 8188, true);
        reg.register(serviceId1, "10.3.1.3", 8188);
        
        String serviceId2 = "test2.xoa.renren.com";
        reg.register(serviceId2, "10.3.1.1", 8188);
        reg.register(serviceId2, "10.3.1.2", 8188);
        reg.register(serviceId2, "10.3.1.3", 8188, true);
        
        reg.queryService(serviceId1);
        reg.queryService(serviceId2);
        
        List<String> ids = reg.lookup("10.3.1.1", 8188);
        Assert.assertEquals(2, ids.size());
        Assert.assertEquals(serviceId1, ids.get(0));
        Assert.assertEquals(serviceId2, ids.get(1));
        
        ids = reg.lookup("10.3.1.2", 8188);
        Assert.assertEquals(1, ids.size());
        Assert.assertEquals(serviceId2, ids.get(0));
        
        ids = reg.lookup("10.3.1.3", 8188);
        Assert.assertEquals(1, ids.size());
        Assert.assertEquals(serviceId1, ids.get(0));
	}
	
	@Test
    public void testListener() {
	    String serviceId1 = "test1.xoa.renren.com";
        SimpleXoaRegistry reg = new SimpleXoaRegistry();
        reg.register(serviceId1, "10.3.1.1", 8188);
        reg.register(serviceId1, "10.3.1.2", 8188, true);
        reg.register(serviceId1, "10.3.1.3", 8188);
        
        String serviceId2 = "test2.xoa.renren.com";
        reg.register(serviceId2, "10.3.1.1", 8188);
        reg.register(serviceId2, "10.3.1.2", 8188);
        reg.register(serviceId2, "10.3.1.3", 8188, true);
        
        reg.queryService(serviceId1);
        reg.queryService(serviceId2);
        
        final XoaServiceDescriptor[] optNodes = new XoaServiceDescriptor[2];
        reg.addListener(new XoaRegistryLogListener());
        reg.addListener(new XoaRegistryListener() {
            
            @Override
            public void onNodeDisabled(XoaServiceDescriptor node) {
                optNodes[0] = node;
            }
            
            @Override
            public void onNodeDeleted(XoaServiceDescriptor node) {
                optNodes[1] = node;
            }
        });

        reg.deleteNode(serviceId1, "10.3.1.1", 8188);
        
        Assert.assertEquals(optNodes[1].getServiceId(), serviceId1);
        Assert.assertEquals(optNodes[1].getIpAddress(), "10.3.1.1");
        Assert.assertEquals(optNodes[1].getPort(), 8188);
        
        
        reg.deleteNode(serviceId2, "10.3.1.3", 8188);
        Assert.assertEquals(optNodes[1].getServiceId(), serviceId2);
        Assert.assertEquals(optNodes[1].getIpAddress(), "10.3.1.3");
        Assert.assertEquals(optNodes[1].getPort(), 8188);
                
        reg.disableNode(serviceId2, "10.3.1.2", 8188);
        
        Assert.assertEquals(optNodes[0].getServiceId(), serviceId2);
        Assert.assertEquals(optNodes[0].getIpAddress(), "10.3.1.2");
        Assert.assertEquals(optNodes[0].getPort(), 8188);
	}
}
