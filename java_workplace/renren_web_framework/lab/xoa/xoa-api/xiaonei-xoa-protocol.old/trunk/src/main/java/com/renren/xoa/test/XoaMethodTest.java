package com.renren.xoa.test;

import org.json.JSONObject;

import com.renren.xoa.JOMapper;
import com.renren.xoa.XoaBizException;
import com.renren.xoa.XoaClient;
import com.renren.xoa.XoaCommunicationException;
import com.renren.xoa.XoaMethod;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.method.XoaDeleteMethod;
import com.renren.xoa.method.XoaGetMethod;
import com.renren.xoa.method.XoaPostMethod;
import com.renren.xoa.method.XoaPutMethod;

public class XoaMethodTest {

	public static String DOMAIN = "127.0.0.1:8080";
	
	public static void testPut() {
		
		XoaMethod method = new XoaPutMethod(DOMAIN, "/spdy/xoa-test?hehe=haha");
		method.setParam("1", "2");
		method.setParam("1", "2");
		method.setParam("1", "6");
		method.setParam("3", "4");
		XoaClient client = new XoaClient();
		try {
			XoaResponse res = client.executeMethod(method);
			System.out.println(res.getContentAsString());
		} catch (XoaCommunicationException e) {
			e.printStackTrace();
		} catch (XoaBizException e) {
			e.printStackTrace();
		}
	}
	
	public static void testPost() {
		
		XoaMethod method = new XoaPostMethod(DOMAIN, "/spdy/xoa-test?hehe=haha");
		method.setParam("1", "2");
		method.setParam("1", "2");
		method.setParam("1", "6");
		method.setParam("3", "4");
		XoaClient client = new XoaClient();
		try {
			XoaResponse res = client.executeMethod(method);
			System.out.println(res.getContentAsString());
		} catch (XoaCommunicationException e) {
			e.printStackTrace();
		} catch (XoaBizException e) {
			e.printStackTrace();
		}
	}
	
	public static void testDelete() {
		
		XoaMethod method = new XoaDeleteMethod(DOMAIN, "/spdy/xoa-test?hehe=haha");
		method.setParam("1", "2");
		method.setParam("3", "4");
		XoaClient client = new XoaClient();
		try {
			XoaResponse res = client.executeMethod(method);
			System.out.println(res.getContentAsString());
		} catch (XoaCommunicationException e) {
			e.printStackTrace();
		} catch (XoaBizException e) {
			e.printStackTrace();
		}
	}
	
	public static void testGet() {
		
		/*XoaMethod method = new GetMethod(DOMAIN, "/spdy/xoa-test?hehe=haha");
		method.setParam("1", "哈哈");
		method.setParam("3", "4");
		XoaClient client = new XoaClient();
		try {
			XoaResponse res = client.executeMethod(method);
			System.out.println(res.getContentAsString());
			res.getContent(new JOMapper() {

				@Override
				public Object map(JSONObject json, Object object) {
					System.out.println(json.toString());
					System.out.println(json.optJSONObject("body-params").opt("1"));
					return null;
				}
			}, null);
		} catch (XoaCommunicationException e) {
			e.printStackTrace();
		} catch (XoaBizException e) {
			e.printStackTrace();
		}*/
	}
	
	
	public static void main(String[] args) {
		testGet();
	}
}
