package com.renren.xoa.registry.impl;

import java.util.ArrayList;
import java.util.List;

import com.renren.xoa.registry.AbstractXoaRegistry;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-28 下午12:33:13
 */
public class MockRegistry extends AbstractXoaRegistry {

	public static final String IP_PREFIX = "10.3.1.";
	
	public static final int IP_MAX = 5;

	public static final int PORT = 8188;
	
	@Override
	public List<XoaServiceDescriptor> getServiceNodes(String serviceId) {

		List<XoaServiceDescriptor> list = new ArrayList<XoaServiceDescriptor>();
		for (int i = 1; i <= IP_MAX; i++) {
			String ip = IP_PREFIX + i;
			XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
			desc.setServiceId(serviceId);
			desc.setIpAddress(ip);
			desc.setPort(PORT);
			list.add(desc);
		}
		return list;
	}

	public void updateServiceNodes(String serviceId,
			List<XoaServiceDescriptor> nodes) {
		super.updateServiceNodes(serviceId, nodes);
	}
	
}
