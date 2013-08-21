package com.renren.xoa.test.simple;

import java.util.Arrays;

import com.renren.xoa.Method;
import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class EncodingTest {

public static void main(String[] args) {
		
		//String url = "xoa://blog.xoa.renren.com/encodingTest?hehe=haha";	//定位xoa服务中的资源的url
		String url = "xoa://blog.xoa.renren.com/echoServlet";
		Method method = Method.get(url);	//获取一个对url指定的资源进行GET操作的method
		//设置parameter
		//method.setParam("tag", "it");	//add a parameter with the name tag
		//method.setParam("tag", "computer");	//add another parameter with the name tag
		//method.setParam("readable", "true");
		//method.setParam("body", "不错");
		method.setParam("title", "首页");
		//method.setHeader("Content-Length", "33");
		
		XoaClient client = getClientWithTestRegistry();	
		
		try {
			XoaResponse response = client.execute(method);
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
	
	private static XoaClient getClientWithTestRegistry() {
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		desc.setIpAddress("10.22.200.140");
		desc.setPort(8188);
		reg.register(desc);
		
		XoaClient client = new XoaClient(reg);
		return client;
	}
}
