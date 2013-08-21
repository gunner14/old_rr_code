package com.renren.xoa;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.client.service.ResponseObserver;
import com.renren.xoa.client.service.SpdyClient;
import com.renren.xoa.client.service.SpdyClientManager;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.commons.exception.XoaException;
import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;
import com.renren.xoa.methods.XoaDeleteMethod;
import com.renren.xoa.methods.XoaEntityEnclosingMethod;
import com.renren.xoa.methods.XoaGetMethod;
import com.renren.xoa.methods.XoaMethodName;
import com.renren.xoa.methods.XoaMultiFormatPostMethod;
import com.renren.xoa.methods.XoaPutMethod;
import com.renren.xoa.registry.XoaRegistry;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.response.DefaultXoaResponse;

/**
 * XOA服务的client。
 * 
 * XoaClient中的每个方法都是线程安全的，所以建议尽量复用同一个XoaClient实例。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:31:40
 */
public class XoaClient implements XoaClientI {

    public static final String VERSION = "XOA-Client/0.42";
    
	public static final String PROTOCOL_HTTP = "http";

	public static final String PROTOCOL_SPDY = "spdy";

	private static final long DEFUALT_TIMEOUT = 2000;
	
	protected static final Log logger = LogFactory.getLog(XoaClient.class);
	
	private XoaRegistry registry;
	
	public XoaClient(){
		this(CustomRegistryBuilder.getInstance().get());
	}
	
	public XoaClient(XoaRegistry registry) {
		this.registry = registry;
	}
	
	/**
	 * 执行指定的xoa方法
	 * 
	 * @param method
	 *            要执行的方法
	 * @return {@link Future}包含XoaResonse对象
	 * @throws IOException
	 * @throws XoaException
	 */
	public Future<XoaResponse> submit(Method method) {
		return submitMethodSpdy(method);
	}
	
	public XoaResponse execute(Method method) {
		return execute(method, DEFUALT_TIMEOUT);
	}
	
	@SuppressWarnings("unchecked")
    public XoaResponse execute(Method method, long timeout) {
	    long startTime = System.currentTimeMillis();
		Future<XoaResponse> future = submit(method);
		try {
			return future.get(timeout, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		} catch (ExecutionException e) {
			throw new RuntimeException(e);
		} catch (TimeoutException e) {
		    long endTime = System.currentTimeMillis();
		    if (future instanceof OperationFuture) {
		        OperationFuture of = (OperationFuture) future;
		        InvocationInfo info = of.invocationInfo;
		        StringBuilder sb = new StringBuilder();
	            sb.append("XOA timeout in ");
	            sb.append(endTime - startTime);
	            sb.append(" ms: ");
	            sb.append(info.getMethodName());
	            sb.append(" ");
	            sb.append(info.getUrl());
	            sb.append(", remote host: ");
	            sb.append(info.getRemoteHost());
	            throw new RuntimeException(sb.toString(), e);
		    } else {
		        //this should never happen
		        throw new RuntimeException("Timed out waiting for operation", e);
		    }
		}
	}
	
	/**
	 * ping一个服务，看有多少服务节点能够正常相应
	 * @param serviceId 服务id
	 * @return 能够正常相应的节点个数
	 */
	public int ping(String serviceId) {
		
	    int ret = 0;
	    
	    List<XoaServiceDescriptor> nodes = registry.queryServices(serviceId);
		
	    if (nodes == null) {
	        return ret;
	    }
	    
		for (XoaServiceDescriptor desc : nodes) {
			String realHost = desc.getIpAddress() + ":" + desc.getPort();
			SpdyClient client = SpdyClientManager.getInstance().getClient(realHost);
			if (client.isConnected()) {
				ret ++;
			}
		}
		return ret;
	}
	
	public InvocationInfo submit(Method xoaMethod, final XoaResponseCallback callback) {
		
		DefaultInvocationInfo info = new DefaultInvocationInfo();
		info.setMethodName(xoaMethod.getName().toString());
		info.setUrl(xoaMethod.getUrl());
		
		String serviceId = xoaMethod.getServiceId();
		XoaServiceDescriptor desc = registry.queryService(serviceId);
		if (desc == null) {
			throw new ServiceNotFoundException(serviceId);
		}
		
		int port = desc.getPort();
		
		//如果xoaMethod中指定了端口，就覆盖掉registry中配置的
		if (xoaMethod.getPort() != -1) {
			port = xoaMethod.getPort();
		}
		
		//hostname + port就是host
		
		//virtual host
		String virtualHost = desc.getServiceId() + ":" + port;
		
		//real host
		String realHost = desc.getIpAddress() + ":" + port;

		SpdyClient client = SpdyClientManager.getInstance().getClient(realHost);
		if (!client.isConnected()) {
			logger.warn("not connect to remote host:" + realHost);
			List<XoaServiceDescriptor> descs = registry.queryServices(serviceId);
			if (descs == null || descs.size() == 0) {
				throw new ServiceNotFoundException(serviceId);
			}
			List<String> realHosts = new ArrayList<String>(descs.size() - 1);
			for (XoaServiceDescriptor tempDesc : descs) {
				if (tempDesc != desc) {
					//自定义端口在这种情况下不支持
					realHosts.add(tempDesc.getIpAddress() + ":" + tempDesc.getPort());
				}
			}
			//从指定的那些real hosts中找到一个可用的
			client = SpdyClientManager.getInstance().findHealthyClient(realHosts);
		}
		
		if (logger.isDebugEnabled()) {
			logger.debug("Find a healthy node of " + serviceId + ": " + client.getRemoteHost());
		}
		
		SpdyHttpRequest shr = wrapSpdyHttpRequest(xoaMethod, virtualHost);
		final String healthyRealHost = client.getRemoteHost();
		
		info.setRemoteHost(healthyRealHost);
		
		client.send(shr, new ResponseObserver() {
			@Override
			public void messageReceived(SpdyHttpResponse response) {
				DefaultXoaResponse xoaResponse = new DefaultXoaResponse();
				xoaResponse.setRemoteHost(healthyRealHost);
				xoaResponse.setStatusCode(response.getStatus().getCode());
				ChannelBuffer buff = response.getContent();
				int length = buff.readableBytes();
				byte[] content = new byte[length];
				buff.readBytes(content);
				xoaResponse.setContent(content);
				for (String headerName : response.getHeaderNames()) {
					xoaResponse.setHeader(headerName, response.getHeader(headerName));
				}
				callback.responseReceived(xoaResponse);
			}
		});
		return info;
	}
	
	private Future<XoaResponse> submitMethodSpdy(Method xoaMethod) {
		final CountDownLatch latch = new CountDownLatch(1);
		final OperationFuture<XoaResponse> of = new OperationFuture<XoaResponse>(
				xoaMethod.getServiceId(), latch, DEFUALT_TIMEOUT);
		InvocationInfo info = submit(xoaMethod, new XoaResponseCallback() {
			@Override
			public void responseReceived(XoaResponse response) {
				of.set(response);
				latch.countDown();
			}
		});
		of.invocationInfo = info;
		return of;
	}
	
	public static interface XoaResponseCallback {
		public void responseReceived(XoaResponse response);
	}
	
	/**
	 * 将一个XoaMethod转换为SpdyHttpRequest
	 * 
	 * @param xoaMethod
	 * @return
	 */
	private SpdyHttpRequest wrapSpdyHttpRequest(Method xoaMethod, String host) {
		
		SpdyHttpRequest shr;
		if (xoaMethod.getName() == XoaMethodName.GET) {
			
			XoaGetMethod getMethod = (XoaGetMethod)xoaMethod;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.GET,
					getMethod.getPathWithFullQueryString());
		} else if (xoaMethod.getName() == XoaMethodName.DELETE){
			
			XoaDeleteMethod deleteMethod = (XoaDeleteMethod)xoaMethod;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.DELETE,
					deleteMethod.getPathWithFullQueryString());
		
		} else if (xoaMethod.getName() == XoaMethodName.PUT){
			XoaPutMethod putMethod = (XoaPutMethod)xoaMethod;
			shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.PUT,
					putMethod.getPathWithFullQueryString());
		} else if (xoaMethod instanceof XoaEntityEnclosingMethod) {
			XoaEntityEnclosingMethod eeMethod = (XoaEntityEnclosingMethod)xoaMethod;
			if (xoaMethod.getName() == XoaMethodName.POST) {
				shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.POST,
						eeMethod.getPath());
			} /*else if(xoaMethod.getName() == XoaMethodName.PUT) {
				shr = new SpdyHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.PUT,
						eeMethod.getPath());
			}*/ else {
				throw new RuntimeException("Unsupported method:" + xoaMethod.getName());
			}
			
			byte[] body = eeMethod.getBody();
			
			//设置Content-Length非常重要，否则服务器端不知道请求何时结束
			shr.setHeader(HttpHeaders.Names.CONTENT_LENGTH, Integer.toString(body.length));
			
			ChannelBuffer content = ChannelBuffers.wrappedBuffer(body);
			shr.setContent(content);
			
			if (xoaMethod instanceof XoaMultiFormatPostMethod) {
			    shr.setHeader(HttpHeaders.Names.CONTENT_TYPE, "application/xoa-multiformat");
			} else {
			    //有body的request才用Content-Type的header
	            shr.setHeader(HttpHeaders.Names.CONTENT_TYPE, "application/x-www-form-urlencoded; charset=UTF-8");
			}
			
			
		} else {
			throw new RuntimeException("Unsupported method:" + xoaMethod.getName());
		}
		
		shr.setHeader(HttpHeaders.Names.HOST, host);
		shr.setHeader(HttpHeaders.Names.ACCEPT, Method
						.buildAcceptHeaderValue(new String[] {
								Method.ACCEPT_TEXT_PLAIN,
								Method.ACCEPT_SERIALIZABLE,
								Method.ACCEPT_PROTOC_BUFF,
								Method.ACCEPT_JSON}));

		/*shr.addHeader(HttpHeaders.Names.ACCEPT_ENCODING, 
				HttpHeaders.Values.GZIP + ", " + HttpHeaders.Values.DEFLATE);*/
		shr.setHeader(HttpHeaders.Names.ACCEPT_ENCODING, HttpHeaders.Values.DEFLATE);
		shr.setHeader(HttpHeaders.Names.ACCEPT_CHARSET, "utf-8");
		shr.setHeader(HttpHeaders.Names.USER_AGENT, VERSION);
		
		//copy headers
		Map<String, String> headers = xoaMethod.getHeaders();
		if (headers != null) {
			for (Entry<String, String> entry : headers.entrySet()) {
				shr.setHeader(entry.getKey(), entry.getValue());
			}
		}
		return shr;
	}
	

	/**
	 * 异步调用的Future
	 * @param <V>
	 */
	private static class OperationFuture<V> implements Future<V> {

		private final CountDownLatch latch;
		
		private final String serviceId;
		
		private V v;
		
		private final long defaultTimeout;
		
		private InvocationInfo invocationInfo;
		
		public OperationFuture(String serviceId, CountDownLatch latch, long defaultTimeout) {
			this.serviceId = serviceId;
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
			
			if (v instanceof XoaResponse) {
				XoaResponse response = (XoaResponse) v;
				if (response.getStatusCode() != 200) {
				    
				    StringBuilder msg = new StringBuilder(100);
				    msg.append("[");
				    msg.append(this.serviceId);
				    msg.append("] Status code ");
				    msg.append(response.getStatusCode());
				    msg.append(" from remote host ");
				    msg.append(response.getRemoteHost());
				    
				    if (response.getStatusCode() == XoaBizErrorBean.STATUS_CODE) {
				        
				        XoaBizErrorBean errorBean = response.getContentAs(XoaBizErrorBean.class);
				        msg.append(", errorCode: ");
				        msg.append(errorBean.getErrorCode());
				        msg.append(", errorMsg: ");
				        msg.append(errorBean.getMessage());
				    }
					throw new StatusNotOkException(msg.toString()).setResponse(response);
				}
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
}
