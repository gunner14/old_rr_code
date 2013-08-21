package com.renren.xoa.test.simple;

import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class RepeatProxyTest {

	private XoaClient client;
	
	public RepeatProxyTest() {
		client = getClientWithTestRegistry();
	}
	
	private static XoaClient getClientWithTestRegistry() {
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		String serviceId = "blog.xoa.renren.com";
		//reg.register(serviceId, "127.0.0.1", 8888);
		reg.register(serviceId, "10.22.200.140", 8288);
		
		/*XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setHostName("blog.xoa.renren.com");
		desc.setIpAddress("10.22.200.138");
		desc.setPort(8888);
		reg.register(desc);
		
		desc = new XoaServiceDescriptorBase();
		desc.setHostName("blog.xoa.renren.com");
		desc.setIpAddress("10.22.200.140");
		desc.setPort(8888);
		reg.register(desc);*/
		
		XoaClient client = new XoaClient(reg);
		return client;
	}
	
	public void runOnce() {
		String url = "xoa://blog.xoa.renren.com/benchmark/2/0";
		Method method = Method.get(url);
		XoaResponse res = client.execute(method, 1000);
		Blog[] blogs = res.getContentAs(Blog[].class);
		System.out.println("successful return:" + blogs.length);
	}
	
	public static void main(String[] args) {
		RepeatProxyTest app = new RepeatProxyTest();
		int n = 10000;
		try {
			while (n-- > 0) {
				try {
					app.runOnce();
					//Thread.sleep(2000);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
