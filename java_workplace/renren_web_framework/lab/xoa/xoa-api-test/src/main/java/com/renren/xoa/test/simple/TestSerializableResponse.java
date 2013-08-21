package com.renren.xoa.test.simple;

import java.util.concurrent.Future;

import com.renren.xoa.Method;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.commons.test.Blog;
import com.renren.xoa.commons.test.RizhiProtos.Rizhi;
import com.renren.xoa.commons.test.RizhiProtos.RizhiList;
import com.renren.xoa.registry.XoaServiceDescriptorBase;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

public class TestSerializableResponse {

	public static void main(String[] args) {
/*XoaClient client = new XoaClient();
		
		//client.testSpdy();
		
		String domain = "comment.xoa.renren.com:8688";
		//String path = "/spdy/xoa-test";
		String path = "/700003448/photo-2100002971/0/comment";
		XoaMethod method = new XoaGetMethod(domain, path);*/

		String url = "xoa://blog.xoa.renren.com/benchmark/2/0";
		//String url = "xoa://blog.xoa.renren.com/text/32/0";
		//String url = "xoa://blog.xoa.renren.com/index2.jsp";
		//String url = "xoa://blog.xoa.renren.com/blog/blogs";
		//XoaMethod method = new PostMethod(domain, path);
		Method method = Method.get(url);
		
		SimpleXoaRegistry reg = new SimpleXoaRegistry();
		XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
		desc.setServiceId("blog.xoa.renren.com");
		desc.setIpAddress("10.22.200.140");
		desc.setPort(8888);
		
		reg.register(desc);
		
		XoaClient client = new XoaClient(reg);
		
		//method.setHeader("user-id", "68464");
		try {
			Future<XoaResponse> f = client.submit(method);
			XoaResponse res = f.get();
			int status = res.getStatusCode();
			if (status == 200) {
				
				//System.out.println(res.getContentAsString().length());
				//System.out.println(res.getContentAsString());
				Blog[] list = res.getContentAs(Blog[].class);
				System.out.println(list.length);
			} else {
				System.err.println(status);
			}
			//System.out.println(res.getContentAsJsonObject());
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			System.exit(0);
		}
	}
}
