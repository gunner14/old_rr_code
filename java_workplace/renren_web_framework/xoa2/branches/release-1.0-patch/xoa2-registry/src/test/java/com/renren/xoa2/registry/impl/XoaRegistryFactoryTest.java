package com.renren.xoa2.registry.impl;

import java.util.List;

import org.junit.Test;

import com.renren.xoa2.registry.Node;
import com.renren.xoa2.registry.XoaRegistry;
import com.renren.xoa2.registry.XoaRegistryFactory;

public class XoaRegistryFactoryTest {
	
	@Test
	public void test() {
		XoaRegistry xoaRegistry = XoaRegistryFactory.getInstance().getDefaultRegistry();
		List<Node> list = xoaRegistry.queryService("com.renren.ad.find", "1+");
		System.out.println(list);
	}

}
