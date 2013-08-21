package com.renren.xoa.mock;

import com.renren.xoa.DefaultInvocationInfo;
import com.renren.xoa.InvocationInfo;
import com.renren.xoa.Method;
import com.renren.xoa.XoaClientI;
import com.renren.xoa.XoaResponse;
import com.renren.xoa.XoaClient.XoaResponseCallback;
import com.renren.xoa.response.DefaultXoaResponse;

public class MockXoaClient implements XoaClientI{

	@Override
	public InvocationInfo submit(Method method, XoaResponseCallback callback) {
		
		DefaultInvocationInfo info = new DefaultInvocationInfo();
		info.setMethodName(method.getName().toString());
		info.setUrl(method.getUrl());
		
		DefaultXoaResponse response = new DefaultXoaResponse();
		response.setContent("ok".getBytes());
		response.setStatusCode(200);
		String remoteHost = "127.0.0.1:8188";
		response.setRemoteHost(remoteHost);
		info.setRemoteHost(remoteHost);
		response.setHeader("Content-Type", "application/json");
		
		callback.responseReceived(response);
		return info;
	}

	public XoaResponse getResponse(Method method) {
		
		String uri  = method.getPath();
		
		return null;
	}
}
