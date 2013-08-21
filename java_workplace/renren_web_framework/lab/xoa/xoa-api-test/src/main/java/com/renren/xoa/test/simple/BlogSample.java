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

public class BlogSample {

	public static void main(String[] args) {
		
		String url = "xoa://blog.ugc.xoa.renren.com/blog";
		Method method = Method.get(url);
		//设置parameter
		method.setParam("blogId", "12345");
		method.setParam("ownerId", "54321");
		
		XoaClient client = new XoaClient();
		/////////////异步调用方式//////////////
		try {
			Future<XoaResponse> future = client.submit(method);
			//do something else
			XoaResponse response = future.get();
			//也可以自定义get操作的timeout: 
			//long timeout = 300;	//ms
			//XoaResponse res = f.get(timeout);
			
			Blog blog = response.getContentAs(Blog.class);
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
