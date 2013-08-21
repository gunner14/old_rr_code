package com.renren.xoa;

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.methods.ByteArrayRequestEntity;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.client.service.CommunicationClient;
import com.renren.xoa.client.service.CommunicationService;
import com.renren.xoa.client.service.ResponseObserver;
import com.renren.xoa.commons.exception.IllegalArgumentException;
import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;
import com.renren.xoa.http.HttpClientFactory;
import com.renren.xoa.method.XoaBodilessMethod;
import com.renren.xoa.method.XoaDeleteMethod;
import com.renren.xoa.method.XoaEntityEnclosingMethod;
import com.renren.xoa.method.XoaGetMethod;
import com.renren.xoa.method.XoaMethodName;
import com.renren.xoa.method.XoaPostMethod;
import com.renren.xoa.method.XoaPutMethod;

/**
 * xoa服务的client
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:31:40
 */
public class XoaClient {

	public static final String PROTOCOL_HTTP = "http";

	public static final String PROTOCOL_SPDY = "spdy";

	private static final long DEFUALT_TIMEOUT = 5000;
	
	/**
	 * 执行指定的xoa方法
	 * 
	 * @param method
	 *            要执行的方法
	 * @return {@link Future}包含XoaResonse对象
	 * @throws IOException
	 * @throws XoaException
	 */
	public Future<XoaResponse> submitMethod(XoaMethod method) {
		return submitMethod(method, PROTOCOL_SPDY);
	}

	public Future<XoaResponse> submitMethod(XoaMethod method, String protocol) {

		if (PROTOCOL_HTTP.equals(protocol)) {
			return null;
		} else if (PROTOCOL_SPDY.equals(protocol)) {
			return submitMethodSpdy(method);
		} else {
			throw new IllegalArgumentException("Unsupported protocol:"
					+ protocol);
		}
	}
	
	private Future<XoaResponse> submitMethodSpdy(XoaMethod method) {
		String domain = method.getDomain();
		
		CommunicationClient client;
		try {
			client = CommunicationService.getInstance()
					.getClient("xoa://" + domain);
		} catch (URISyntaxException e) {
			throw new RuntimeException(e);
		}
		
		SpdyHttpRequest shr;
		if (method.getName() == XoaMethodName.GET) {
			
			XoaGetMethod getMethod = (XoaGetMethod)method;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.GET,
					getMethod.getPathWithFullQueryString());
		} else if (method.getName() == XoaMethodName.DELETE){
			
			XoaDeleteMethod deleteMethod = (XoaDeleteMethod)method;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.DELETE,
					deleteMethod.getPathWithFullQueryString());
		
		} else if (method.getName() == XoaMethodName.POST) {
			
			XoaPostMethod postMethod = (XoaPostMethod)method;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.POST,
					postMethod.getPath());
			byte[] body = postMethod.getBody();
			ChannelBuffer content = ChannelBuffers.directBuffer(body.length);
			content.writeBytes(body);
			shr.setContent(content);
		} else if (method.getName() == XoaMethodName.PUT) {
			
			XoaPutMethod putMethod = (XoaPutMethod)method;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.PUT,
					putMethod.getPath());
			byte[] body = putMethod.getBody();
			ChannelBuffer content = ChannelBuffers.directBuffer(body.length);
			content.writeBytes(body);
			shr.setContent(content);
		} else {
			throw new RuntimeException("Unsupported method:" + method.getName());
		}
		
		shr.addHeader(HttpHeaders.Names.ACCEPT_ENCODING, 
				HttpHeaders.Values.GZIP + ", " + HttpHeaders.Values.DEFLATE);
		shr.addHeader(HttpHeaders.Names.ACCEPT_CHARSET, "utf-8");
		shr.addHeader(HttpHeaders.Names.USER_AGENT, "XOA-Client");
		
		final CountDownLatch latch = new CountDownLatch(1);
		final OperationFuture<XoaResponse> of = new OperationFuture<XoaResponse>(
				latch, DEFUALT_TIMEOUT);
		client.send(shr, new ResponseObserver() {
			
			@Override
			public void messageReceived(SpdyHttpResponse response) {
				DefaultXoaResponse xoaResponse = new DefaultXoaResponse();
				xoaResponse.setStatusCode(response.getStatus().getCode());
				ChannelBuffer buff = response.getContent();
				int length = buff.readableBytes();
				byte[] content = new byte[length];
				buff.readBytes(content);
				xoaResponse.setContent(content);
				for (String headerName : response.getHeaderNames()) {
					xoaResponse.setHeader(headerName, response.getHeader(headerName));
				}
				of.set(xoaResponse);
				latch.countDown();
			}
		});
		return of;
	}
	
	public XoaResponse executeMethod(XoaMethod method) throws XoaCommunicationException, XoaBizException {
		return executeMethod(method, PROTOCOL_HTTP);
	}
	
	private XoaResponse executeMethod(XoaMethod method, String protocol) throws XoaCommunicationException, XoaBizException {
		if (PROTOCOL_HTTP.equals(protocol)) {
			return executeMethodHttp(method);
		} else if (PROTOCOL_SPDY.equals(protocol)) {
			return null;
		} else {
			throw new IllegalArgumentException("Unsupported protocol:"
					+ protocol);
		}
	}

	private XoaResponse executeMethodHttp(XoaMethod xoaMethod) throws XoaCommunicationException, XoaBizException {
		
		org.apache.commons.httpclient.HttpMethod httpMethod;
		
		if (xoaMethod instanceof XoaBodilessMethod) {
			
			XoaBodilessMethod bodilessMethod = (XoaBodilessMethod) xoaMethod;
			String uri = "http://" + bodilessMethod.getDomain() + bodilessMethod.getPathWithFullQueryString();
			if (xoaMethod.getName() == XoaMethodName.GET) {
				httpMethod = new org.apache.commons.httpclient.methods.GetMethod(uri);
			} else if (xoaMethod.getName() == XoaMethodName.DELETE) {
				httpMethod = new org.apache.commons.httpclient.methods.DeleteMethod(uri);
			} else {
				throw new RuntimeException("Unsupported method:" + xoaMethod.getName());
			}
			
		} else if (xoaMethod instanceof XoaEntityEnclosingMethod) {
			
			String uri = "http://" + xoaMethod.getDomain() + xoaMethod.getPath();
			XoaEntityEnclosingMethod xoaEEMethod = (XoaEntityEnclosingMethod)xoaMethod;
			byte[] body = xoaEEMethod.getBody();
			org.apache.commons.httpclient.methods.EntityEnclosingMethod httpEEMethod;
			if (xoaMethod.getName() == XoaMethodName.PUT) {
				httpEEMethod = new org.apache.commons.httpclient.methods.PutMethod(uri);
			} else if (xoaMethod.getName() == XoaMethodName.POST) {
				httpEEMethod = new org.apache.commons.httpclient.methods.PostMethod(uri);
			} else {
				throw new RuntimeException("Unsupported method:" + xoaMethod.getName());
			}
			ByteArrayRequestEntity entity = new ByteArrayRequestEntity(body, "application/x-www-form-urlencoded");
			httpEEMethod.setRequestEntity(entity);
			httpMethod = httpEEMethod;
		} else {
			throw new IllegalArgumentException(xoaMethod.getClass().getName());
		}
		
		HttpClient client = HttpClientFactory.getInstance().getHttpClient();
		try {
			client.executeMethod(httpMethod);
			if (httpMethod.getStatusCode() == Constants.STATUS_OK) {
				
				Header headerBizCode = httpMethod.getResponseHeader(Constants.HEADER_BIZ_CODE);
				if (headerBizCode != null) {
					int bizCode = Integer.parseInt(headerBizCode.getValue());
					String bizMessage = httpMethod.getResponseBodyAsString();
					throw new XoaBizException(bizCode, bizMessage);
				}
				
				DefaultXoaResponse response = new DefaultXoaResponse();
				response.setContent(httpMethod.getResponseBody());
				return response;
			} else {
				throw new XoaCommunicationException("Server response status:" + httpMethod.getStatusCode());
			}
		} catch (HttpException e) {
			throw new XoaCommunicationException(e);
		} catch (IOException e) {
			throw new XoaCommunicationException(e);
		}
	}

	private static class OperationFuture<V> implements Future<V> {

		private final CountDownLatch latch;
		
		private V v;
		
		private final long defaultTimeout;
		
		public OperationFuture(CountDownLatch latch, long defaultTimeout) {
			this.latch = latch;
			this.defaultTimeout = defaultTimeout;
		}
		
		@Override
		public boolean cancel(boolean mayInterruptIfRunning) {
			return false;
		}

		public void set(V v) {
			this.v = v;
		}
		
		@Override
		public V get() throws InterruptedException, ExecutionException {
			try {
				return get(defaultTimeout, TimeUnit.MILLISECONDS);
			} catch (TimeoutException e) {
				throw new RuntimeException("Timed out waiting for operation", e);
			}
		}
		
		@Override
		public V get(long timeout, TimeUnit units)
				throws InterruptedException, TimeoutException,
				ExecutionException {
			if (!latch.await(timeout, units)) {
				throw new TimeoutException("Timed out waiting for operation");
			}
			
			if (isCancelled()) {
				throw new ExecutionException(new RuntimeException("Cancelled"));
			}

			return v;
		}
		
		@Override
		public boolean isCancelled() {
			return false;
		}

		@Override
		public boolean isDone() {
			return latch.getCount() == 0;
		}
	}
	
	public static void main(String[] args) {
		
		XoaClient client = new XoaClient();
		
		//client.testSpdy();
		
		String domain = "127.0.0.1:8888";
		//String path = "/spdy/xoa-test";
		String path = "/xoa-demo/blog";
		XoaMethod method = new XoaGetMethod(domain, path);
		try {
			Future<XoaResponse> f = client.submitMethod(method);
			XoaResponse res = f.get();
			System.out.println(res.getContentAsJson());
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		System.exit(0);
	}
}
