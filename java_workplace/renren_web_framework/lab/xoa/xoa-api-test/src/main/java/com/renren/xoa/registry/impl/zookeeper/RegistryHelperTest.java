package com.renren.xoa.registry.impl.zookeeper;

import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.zookeeper.ZooKeeperFactory;

public class RegistryHelperTest {

	public static void addNode() {
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setIpAddress("10.3.1.8");
		desc.setServiceId("test.xoa.renren.com");
		desc.setPort(8189);
		desc.setDisabled(false);
		RegistryHelper.registerServiceNode(desc);
	}
	
	public static void disableNode() {
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setIpAddress("10.3.1.7");
		desc.setServiceId("test.xoa.renren.com");
		desc.setPort(8189);
		desc.setDisabled(false);
		RegistryHelper.updateServiceNode(desc);
	}
	
	public static void deleteNode() {
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setIpAddress("10.3.1.5");
		desc.setServiceId("test.xoa.renren.com");
		desc.setPort(8189);
		
		RegistryHelper.deleteServiceNode(desc);
	}
	
	public static void main(String[] args) {
		
		ZooKeeperFactory.HOSTS = "10.22.200.140:2181";
		deleteNode();
	}
	
}
