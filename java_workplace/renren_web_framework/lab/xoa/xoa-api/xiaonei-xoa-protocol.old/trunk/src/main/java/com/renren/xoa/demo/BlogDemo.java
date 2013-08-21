package com.renren.xoa.demo;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.json.JSONObject;

import com.renren.xoa.JOMapper;
import com.renren.xoa.XoaBizException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaCommunicationException;
import com.renren.xoa.XoaMethod;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.method.XoaGetMethod;
import com.renren.xoa.method.XoaPostMethod;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-12 下午03:34:59
 */
public class BlogDemo {

	private static void test() {
		
		String domain = "blog.renren.com";
		String path = "/xoa-demo/blog?hehe=haha";
		
		XoaMethod method = new XoaPostMethod(domain, path);
		//XoaMethod method = new GetMethod(domain, path);
		method.setParam("1", "2");
		method.setParam("1", "2");
		XoaClient client = new XoaClient();
		try {
			XoaResponse response = client.executeMethod(method);
			Blog blog = response.getContent(new JOMapper<Blog>() {
				@Override
				public Blog map(JSONObject json) {
					Blog blog = new Blog();
					blog.setOwnerId(json.optInt("ownerId"));
					blog.setTitle(json.optString("title"));
					blog.setContent(json.optString("content"));
					return blog;
				}
			});
			
			System.out.println(blog.toString());
			
		} catch (XoaCommunicationException e) {
			e.printStackTrace();
		} catch (XoaBizException e) {
			System.out.println(e.getBizCode() + "-" + e.getMessage());
		}
	}
	
	private static void testSpdy() {
		
		String domain = "127.0.0.1:8888";
		String path = "/xoa-demo/blog";
		
		//XoaMethod method = new PostMethod(domain, path);
		XoaMethod method = new XoaGetMethod(domain, path);
		XoaClient client = new XoaClient();
		
		Future<XoaResponse> f = client.submitMethod(method);
		XoaResponse response;
		try {
			response = f.get();
			Blog blog = response.getContent(new JOMapper<Blog>() {
				@Override
				public Blog map(JSONObject json) {
					Blog blog = new Blog();
					blog.setOwnerId(json.optInt("ownerId"));
					blog.setTitle(json.optString("title"));
					blog.setContent(json.optString("content"));
					return blog;
				}
			});
			
			System.out.println(blog.toString());
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		testSpdy();
		//test();
	}
}
