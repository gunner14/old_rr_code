package com.renren.xoa.test.simple;

import java.util.Arrays;

import com.renren.xoa.Method;
import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.client.spdy.netty.ChannelFactoryManager;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class EchoTest {

	public static void main(String[] args) {
		
		String url = "xoa://blog.xoa.renren.com/echo/method?hehe=haha";	//定位xoa服务中的资源的url
		Method method = Method.put(url);	//获取一个对url指定的资源进行GET操作的method
		//设置parameter
		method.setParam("tag", "it");	//add a parameter with the name tag
		method.setParam("tag", "computer");	//add another parameter with the name tag
		method.setParam("body", getString(55));
		//method.setHeader("Content-Length", "33");
		
		XoaClient client = getClientWithTestRegistry();	
		
		//System.out.println("ping=" + client.ping("blog.xoa.renren.com"));
		
		//XoaClient提供了同步和异步两种调用方式，您可以根据自己的需要灵活选择，示例如下：
		
		/////////////同步调用方式//////////////
		try {
			XoaResponse response = client.execute(method, 500000);
			String content = response.getContentAsString();	
			System.out.println(content.length());
			System.out.println(content);
			System.out.println("done");
		} catch (StatusNotOkException e) {
			XoaResponse response = e.getResponse();
			System.out.println(response.getStatusCode());
		} catch(Throwable e) {
			e.printStackTrace();
		} finally {
			System.exit(0);
		}
	}
	
	public static String getString(int length) {
		char[] cs = new char[length];
		Arrays.fill(cs, '好');
		return new String(cs);
	}
	
	private static XoaClient getClientWithTestRegistry() {
		//return new XoaClient();
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		reg.register("blog.xoa.renren.com", "10.22.200.140", 8188);
		/*XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		desc.setIpAddress("10.22.200.140");
		desc.setPort(8188);
		reg.register(desc);*/
		
		XoaClient client = new XoaClient(reg);
		return client;
	}
	
}
