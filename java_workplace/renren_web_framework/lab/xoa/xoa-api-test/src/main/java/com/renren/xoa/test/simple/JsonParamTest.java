package com.renren.xoa.test.simple;

import com.renren.xoa.Method;
import com.renren.xoa.StatusNotOkException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class JsonParamTest {

	public static void main(String[] args) {
		
		String url = "xoa://blog.xoa.renren.com/jsonparam/jsonParam";	//定位xoa服务中的资源的url
		Method method = Method.post(url);	//获取一个对url指定的资源进行GET操作的method
		
		Blog blog = new Blog();
		blog.setContent("这是正文");
		blog.setTitle("这是标题");
		blog.setOwnerId(1234);
		method.setParamAsJson("blog", blog);
		
		XoaClient client = getClientWithTestRegistry();	
		
		//XoaClient提供了同步和异步两种调用方式，您可以根据自己的需要灵活选择，示例如下：
		
		/////////////同步调用方式//////////////
		try {
			XoaResponse response = client.execute(method);
			String content = response.getContentAsString();	
			System.out.println(content.length());
			System.out.println(content);
			System.out.println("done");
		} catch (StatusNotOkException e) {
			XoaResponse response = e.getResponse();
			System.out.println(response.getStatusCode());
			System.out.println(response.getContentAsString());
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
