package com.renren.xoa.test.simple;

import static org.junit.Assert.*;

import org.junit.Test;

import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.RizhiProtos.Rizhi;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class ResponseContentTypeTest {
	
	@Test
	public void defaultTest() {
		String url = "xoa://blog.xoa.renren.com/benchmark/2/0";
		Method method = Method.get(url);		
		XoaClient client = getClientWithTestRegistry();
		XoaResponse res = client.execute(method);
		assertTrue(res.getHeader("Content-Type").contains(Method.ACCEPT_SERIALIZABLE));
		Blog[] blogs = res.getContentAs(Blog[].class);
		assertEquals(blogs.length, 20);
	}
	
	@Test
	public void serialTest() {
		String url = "xoa://blog.xoa.renren.com/benchmark/2/0";
		Method method = Method.get(url);		
		method.setAcceptHeader(Method.ACCEPT_SERIALIZABLE);
		XoaClient client = getClientWithTestRegistry();
		XoaResponse res = client.execute(method);
		assertTrue(res.getHeader("Content-Type").contains(Method.ACCEPT_SERIALIZABLE));
		Blog[] blogs = res.getContentAs(Blog[].class);
		assertEquals(blogs.length, 20);
	}
	
	@Test
	public void jsonTest() {
		String url = "xoa://blog.xoa.renren.com/benchmark/2/0";
		Method method = Method.get(url);		
		method.setAcceptHeader(Method.ACCEPT_JSON);
		XoaClient client = getClientWithTestRegistry();
		XoaResponse res = client.execute(method);
		//System.out.println(res.getHeader("Content-Type"));
		assertTrue(res.getHeader("Content-Type").contains(Method.ACCEPT_JSON));
		Blog[] blogs = res.getContentAs(Blog[].class);
		assertEquals(blogs.length, 20);
	}
	
	@Test
	public void protocBuffTest() {
		String url = "xoa://blog.xoa.renren.com/protoBenchmark/2/0";
		Method method = Method.get(url);		
		method.setAcceptHeader(Method.ACCEPT_PROTOC_BUFF);
		XoaClient client = getClientWithTestRegistry();
		XoaResponse res = client.execute(method);
		//System.out.println(res.getHeader("Content-Type"));
		//System.out.println(res.getContentAsString());
		assertTrue(res.getHeader("Content-Type").contains(Method.ACCEPT_PROTOC_BUFF));
		RizhiList rizhis = res.getContentAs(RizhiList.class);
		assertEquals(rizhis.getRizhisCount(), 20);
	}
	
	private static XoaClient getClientWithTestRegistry() {
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		desc.setIpAddress("60.28.212.140");
		desc.setPort(8888);
		reg.register(desc);
		
		XoaClient client = new XoaClient(reg);
		return client;
	}
	
}
