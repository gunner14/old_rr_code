package com.renren.xoa.test.simple;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class SimpleTest {

	public static void main(String[] args) {
		
		String url = "xoa://blog.xoa.renren.com/blog";	//定位xoa服务中的资源的url
		Method method = Method.get(url);	//获取一个对url指定的资源进行GET操作的method
		//设置parameter
		method.setParam("tag", "it");	//add a parameter with the name tag
		method.setParam("tag", "computer");	//add another parameter with the name tag
		method.setParam("readable", "true");
		 
		//设置header
		//method.setHeader("user-id", "12345");	//如果服务端Controller里希望通过hostHolder获取当前用户，则需要设置一个user-id的header
		
		XoaClient client = getClientWithTestRegistry();	//构造一个XoaClient, XoaClient中的每个方法都是线程安全的，所以建议尽量复用同一个XoaClient实例。
		
		//XoaClient提供了同步和异步两种调用方式，您可以根据自己的需要灵活选择，示例如下：
		
		/////////////异步调用方式//////////////
		try {
			Future<XoaResponse> future = client.submit(method);	//提交XoaMethod
			//do something else
			XoaResponse response = future.get();	//从Future拿到XoaResponse
			//也可以自定义get操作的timeout: 
			//long timeout = 300;	//ms
			//XoaResponse res = f.get(timeout);
			
			Blog blog = response.getContentAs(Blog.class);	//将XoaResponse的content还原为blog对象
			System.out.println(blog.toString());
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		//////////////////////////////////////
		
		/////////////同步调用方式//////////////
		//XoaResponse response = client.execute(method);
		//Blog blog = response.getContentAs(Blog.class);	//将XoaResponse的content还原为blog对象
		//System.out.println(blog.toString());
		//////////////////////////////////////
		
		//System.exit(0);
	}
	
	private static XoaClient getClientWithTestRegistry() {
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		//desc.setIpAddress("60.28.212.140");
		desc.setIpAddress("127.0.0.1");
		desc.setPort(8888);
		reg.register(desc);
		
		XoaClient client = new XoaClient(reg);
		return client;
	}
	
	public static void test(String[] args) {
		
		
		int[] blogIds = new int[]{1111,2222,3333};
		XoaClient client = new XoaClient();
		String urlPrefix = "xoa://blog.xoa.renren.com/blog/";
		List<Future<XoaResponse>> futures = new ArrayList<Future<XoaResponse>>(blogIds.length);
		for (int blogId : blogIds) {
			Method method = Method.get(urlPrefix + blogId);//获取一个对url指定的资源进行GET操作的method
			Future<XoaResponse> future = client.submit(method);	//提交XoaMethod
			futures.add(future);	//保存Future对象
		}
		
		List<Blog> blogs = new ArrayList<Blog>(blogIds.length);
		for (Future<XoaResponse> future : futures) {
			try {
				XoaResponse response = future.get();
				Blog blog = response.getContentAs(Blog.class);	//将XoaResponse的content还原为blog对象
				blogs.add(blog);	//从Future拿到XoaResponse
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
			}
		}
	}
}
