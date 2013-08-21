package com.renren.xoa.registry.tomcat;

import java.net.InetAddress;
import java.util.List;

import junit.framework.Assert;

import org.junit.Test;

import com.renren.xoa.registry.XoaServiceDescriptor;

public class ServerXmlResolverTest {

	@Test
	public void test() throws Exception {
		
		ServerXmlResolver resolver = new ServerXmlResolver();

		List<XoaServiceDescriptor> services = resolver.resolve(this.getClass()
				.getResourceAsStream("server.xml"));
		
		Assert.assertNotNull(services);
		
		
		String ip = InetAddress.getLocalHost().getHostAddress();
		
		XoaServiceDescriptor node0 = services.get(0);
		Assert.assertEquals("photo.ugc.xoa.renren.com", node0.getServiceId());
		Assert.assertEquals(ip, node0.getIpAddress());
		Assert.assertEquals(8188, node0.getPort());
		
		XoaServiceDescriptor node1 = services.get(1);
		Assert.assertEquals("entry.xoa.renren.com", node1.getServiceId());
		Assert.assertEquals(ip, node1.getIpAddress());
		Assert.assertEquals(8188, node1.getPort());
		
		
		XoaServiceDescriptor node2 = services.get(2);
		Assert.assertEquals("forum.ugc.xoa.renren.com", node2.getServiceId());
		Assert.assertEquals(ip, node2.getIpAddress());
		Assert.assertEquals(8188, node2.getPort());
	}
	
}
