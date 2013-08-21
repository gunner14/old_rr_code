package com.renren.xoa.server.coyote;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.util.List;
import java.util.Set;

import org.apache.coyote.ActionCode;
import org.apache.coyote.ActionHook;
import org.apache.coyote.InputBuffer;
import org.apache.coyote.OutputBuffer;
import org.apache.coyote.Request;
import org.apache.coyote.Response;
import org.apache.log4j.Logger;
import org.apache.tomcat.util.buf.ByteChunk;
import org.apache.tomcat.util.buf.MessageBytes;
import org.apache.tomcat.util.http.MimeHeaders;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpResponse;
import org.jboss.netty.handler.codec.http.HttpResponseStatus;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;
import com.renren.xoa.server.ServerContext;
import com.renren.xoa.server.netty.NettyResponseSender;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-2-25 下午03:10:39
 */
public class NettyRequestAdapter implements ActionHook {

	public static final Logger logger = Logger
			.getLogger(NettyRequestAdapter.class);

	private static final int DEFAULT_PORT = 80;

	private Request coyoteRequst;

	private Response coyoteResponse;

	private HttpRequest nettyRequest;

	private HttpResponse nettyResponse;

	private MessageEvent messageEvent;

	private ServerContext serverContext;

	private int streamId;

	//private OutputBuffer outputBuffer = new InternalSpdyOutputBuffer(this);
	
	private OutputBuffer outputBuffer = new InternalSpdyWrapOutputBuffer(this);
	
	private NettyRequestAdapter() {

	}

	public static NettyRequestAdapter construct(MessageEvent event,
			ServerContext serverContext) {

		NettyRequestAdapter ret = new NettyRequestAdapter();
		//TODO 因为初始化顺序的原因，要先new在init，将来这里可以调整
		ret.init(event, serverContext);
		return ret;
	}

	private void init(MessageEvent messageEvent, ServerContext serverContext) {
		this.messageEvent = messageEvent;
		this.nettyRequest = (HttpRequest) messageEvent.getMessage();
		if (messageEvent.getMessage() instanceof SpdyHttpRequest) {
			streamId = ((SpdyHttpRequest) messageEvent.getMessage())
					.getStreamId();
		}
		this.serverContext = serverContext;
		coyoteRequst = new Request();
		coyoteResponse = new Response();
		coyoteRequst.setResponse(coyoteResponse);
		coyoteResponse.setRequest(coyoteRequst);
		coyoteResponse.setHook(this);
		// TODO 以下代码中所有注释掉的set方法可能都要实现吧

		// req.setAvailable();
		// req.setBytesRead(??);
		/*
		String contentEncoding = nettyRequest
				.getHeader(HttpHeaders.Names.CONTENT_ENCODING);
		if (logger.isTraceEnabled()) {
			logger.trace("nettyRequest.contentEncoding=" + contentEncoding);
		}
		if (contentEncoding != null) {
			coyoteRequst.setCharacterEncoding(contentEncoding);
		}
		coyoteRequst.setContentLength((int) nettyRequest.getContentLength(-1));
		
		String contentType = nettyRequest
				.getHeader(HttpHeaders.Names.CONTENT_TYPE);
		if (logger.isTraceEnabled()) {
			logger.trace("nettyRequest.contentType=" + contentType);
		}
		if (contentType != null) {
			//coyoteRequst.setContentType(contentType);
			wrap(coyoteRequst.contentType(), contentType);
		}*/
		
		Channel channel = messageEvent.getChannel();
		
		//wrap local address
		SocketAddress localAddr = channel.getLocalAddress();
		if (localAddr instanceof InetSocketAddress) {
			InetSocketAddress addr = (InetSocketAddress)localAddr;
			coyoteRequst.setLocalPort(addr.getPort());
			wrap(coyoteRequst.localAddr(), addr.getAddress().getHostAddress());
			wrap(coyoteRequst.localName(), addr.getHostName());
		}
		
		//wrap remote address
		SocketAddress remoteAddr = channel.getRemoteAddress();
		if (remoteAddr instanceof InetSocketAddress) {
			InetSocketAddress addr = (InetSocketAddress)remoteAddr;
			coyoteRequst.setRemotePort(addr.getPort());
			wrap(coyoteRequst.remoteAddr(), addr.getAddress().getHostAddress());
			wrap(coyoteRequst.remoteHost(), addr.getHostName());
		}

		coyoteRequst.setInputBuffer(inputBuffer);
		coyoteRequst.setResponse(coyoteResponse);
		coyoteRequst.setServerPort(serverContext.getPort());
		coyoteRequst.setStartTime(System.currentTimeMillis());

		coyoteResponse.setOutputBuffer(outputBuffer);

		String uri = nettyRequest.getUri();

		if (logger.isDebugEnabled()) {
			logger.debug("uri from nettyRequest:" + uri);
		}

		int index = uri.indexOf('?');
		String path = uri;
		String queryString = "";
		if (index != -1) {
			path = uri.substring(0, index);
			if (uri.length() > index + 1) { // ?后面还有东西
				queryString = uri.substring(index + 1);
			}
		}

		//如果访问xoa://abc.com，此时path为""，而tomcat无法处理path为""的情况，所以
		//必须改写为"/"
		if (path == null || path.length() == 0) {
			path = "/";
		}
		wrap(coyoteRequst.requestURI(), path);
		if (logger.isDebugEnabled()) {
			logger.debug("wrap to coyoteRequst.requestURI():" + path);
		}
		wrap(coyoteRequst.decodedURI(), path);
		wrap(coyoteRequst.method(), nettyRequest.getMethod().toString());
		if (logger.isDebugEnabled()) {
			logger.debug("wrap to coyoteRequst.method():"
					+ nettyRequest.getMethod().toString());
		}
		wrap(coyoteRequst.protocol(), nettyRequest.getProtocolVersion()
				.toString());
		if (logger.isDebugEnabled()) {
			logger.debug("wrap to coyoteRequst.protocol():"
					+ nettyRequest.getProtocolVersion().toString());
		}
		wrap(coyoteRequst.queryString(), queryString);
		if (logger.isDebugEnabled()) {
			logger.debug("wrap to coyoteRequst.queryString():" + queryString);
		}

		String serverName = nettyRequest.getHeader(HttpHeaders.Names.HOST);
		if (serverName != null) {
			int offset = serverName.indexOf(':');

			int port = DEFAULT_PORT;
			if (offset != -1) {
				port = Integer.parseInt(serverName.substring(offset + 1));
				serverName = serverName.substring(0, offset);
			}

			if (logger.isDebugEnabled()) {
				logger.debug("set serverName to coyoteRequst:" + serverName);
			}
			coyoteRequst.serverName().setString(serverName);

			coyoteRequst.setServerPort(port);
			if (logger.isDebugEnabled()) {
				logger.debug("set serverPort to coyoteRequst:" + port);
			}
		}

		// copy headers
		MimeHeaders coyoteHeaders = coyoteRequst.getMimeHeaders();
		Set<String> headerNames = nettyRequest.getHeaderNames();
		for (String headerName : headerNames) {
			List<String> headerValues = nettyRequest.getHeaders(headerName);
			for (String headerValue : headerValues) {
				MessageBytes val = coyoteHeaders.addValue(headerName);
				val.setString(headerValue);
				if (logger.isDebugEnabled()) {
					logger.debug("copy header->" + headerName + ":"
							+ headerValue);
				}
			}
		}
	}

	private void wrap(MessageBytes mb, String s) {
		//mb.setString(s);
		//System.out.println("==before:" + mb.toString());
		byte[] bytes = s.getBytes();
		mb.setBytes(bytes, 0, bytes.length);
		//System.out.println("==after:" + mb.toString());
	}

	private InputBuffer inputBuffer = new InputBuffer() {

		@Override
		public int doRead(ByteChunk bchunk, Request request) throws IOException {
			
			try {
				ChannelBuffer content = nettyRequest.getContent();
				int length = content.readableBytes();
				int limit = bchunk.getLimit();	//bchunk的存储上限，即其size
				if (logger.isDebugEnabled()) {
					logger.debug("bytes to doRead:" + length + " bytes");
					//logger.debug("ByteChunk: offset=" + bchunk.getOffset() + ",limit=" + bchunk.getLimit());
				}
				
				int bytesRead = 0;	//本次读出的字节数
				bchunk.setOffset(0);	//将buffer的offset置为0，服务器传过来的对象是复用上次的，不会自动重置
				if (length <= limit) {	//本次能读完
					content.readBytes(bchunk.getBytes(), 0, length);
					bchunk.setEnd(length);
					bytesRead = length;
				} else {	//本次读不完
					content.readBytes(bchunk.getBytes(), 0, limit);
					bchunk.setEnd(limit);
					bytesRead = limit;
				}
				if (logger.isDebugEnabled()) {
					logger.debug("bytes read:" + bytesRead);
					logger.debug("content read: "
							+ new String(bchunk.getBuffer(), bchunk.getStart(),
									bchunk.getLength()));
				}
				return bytesRead;
			} catch (Throwable e) {
				e.printStackTrace();
			}
			return 0;
		}
	};
	
	public Request getCoyoteRequst() {
		return coyoteRequst;
	}

	public HttpResponse getNettyResponse() {
		return nettyResponse;
	}

	public void setNettyResponse(HttpResponse nettyResponse) {
		this.nettyResponse = nettyResponse;
	}

	public MessageEvent getMessageEvent() {
		return messageEvent;
	}

	public void setMessageEvent(MessageEvent messageEvent) {
		this.messageEvent = messageEvent;
	}

	public Response getCoyoteResponse() {
		return coyoteResponse;
	}

	/**
	 * 如果nettyResponse尚未初始化，就初始化一个
	 * 
	 * @param response
	 */
	private void initSpdyHttpResponseIfNeed(Response response) {
		if (nettyResponse == null) {
			nettyResponse = new SpdyHttpResponse(HttpVersion.HTTP_1_1,
					HttpResponseStatus.valueOf(response.getStatus()),
					this.streamId);
			MimeHeaders coyoteHeaders = coyoteResponse.getMimeHeaders();
			for (int i = 0; i < coyoteHeaders.size(); i++) {
				MessageBytes headerName = coyoteHeaders.getName(i);
				MessageBytes headerValue = coyoteHeaders.getValue(i);
				nettyResponse.addHeader(headerName.toString(), headerValue
						.toString());
			}

			String contentType = coyoteResponse.getContentType();
			if (logger.isTraceEnabled()) {
				logger.trace("coyoteResponse.getContentType()="
						+ coyoteResponse.getContentType());
			}
			if (contentType != null) {
				nettyResponse.addHeader("Content-Type", contentType);
			}
			String contentLang = coyoteResponse.getContentLanguage();
			if (contentLang != null) {
				nettyResponse.addHeader("Content-Language", contentLang);
			}

			int contentLength = coyoteResponse.getContentLength();
			if (logger.isTraceEnabled()) {
				logger.trace("coyoteResponse.getContentLength()="
						+ coyoteResponse.getContentLength());
			}
			if (contentLength != -1) {
				nettyResponse.addHeader("Content-Length", contentLength + "");
			}
		}
	}

	@Override
	public void action(ActionCode actionCode, Object param) {

		/*
		 * if (logger.isTraceEnabled()) { logger.trace("actionCode=" +
		 * actionCode.getCode()); }
		 */
		if (actionCode == ActionCode.ACTION_COMMIT) { // commit了，要把headers发出去
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_COMMIT");
			}
			// 如果client(servlet)中没有显示调用flush()来commit请求，那么在请求结束
			// 前服务器程序会自动commit，commit时并不判断response是否已经被其他程序
			// 如OutputBuffer commit过了，所以这里要判断一下，防止重复commit
			if (coyoteResponse.isCommitted()) {
				if (logger.isTraceEnabled()) {
					logger.trace("coyoteResponse has already been committed");
				}
				return;
			}
			initSpdyHttpResponseIfNeed(coyoteResponse);
			coyoteResponse.setCommitted(true);

		} else if (actionCode == ActionCode.ACTION_ACK) {

			// Acknowlege request，报告收到请求，及请求头部已经读完
			// 除了100-continue的情况，不许要特殊处理，如果可能有100，可以如下处理：
			// Send a 100 status back if it makes sense (response not committed
			// yet, and client specified an expectation for 100-continue)

			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_ACK");
			}
		} else if (actionCode == ActionCode.ACTION_CLIENT_FLUSH) {

			// TODO checkout what does this code do exactly
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_CLIENT_FLUSH");
			}

		} else if (actionCode == ActionCode.ACTION_CLOSE) {

			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_CLOSE");
			}

			// 结束请求
			((SpdyHttpResponse) nettyResponse).setFinished(true);
			NettyResponseSender.send(messageEvent, nettyResponse, false);

		} else if (actionCode == ActionCode.ACTION_RESET) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_RESET");
			}
			// Reset response
			// Note: This must be called before the response is committed
			nettyRequest.getContent().clear();
			logger
					.warn("ActionCode.ACTION_RESET may not been properly handled");
		} else if (actionCode == ActionCode.ACTION_POST_REQUEST) {
			// Hook called after request, but before recycling. Can be used
			// for logging, to update counters, custom cleanup - the request
			// is still visible

			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_POST_REQUEST");
			}

		} else if (actionCode == ActionCode.ACTION_CUSTOM) {
			// Do nothing
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_CUSTOM");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_HOST_ADDR_ATTRIBUTE) {
			// req.remoteAddr().setString(httpReq.remoteAddr().toString());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_HOST_ADDR_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_LOCAL_NAME_ATTRIBUTE) {
			// req.localName().setString(httpReq.localName().toString());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_LOCAL_NAME_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_HOST_ATTRIBUTE) {
			// req.remoteHost().setString(httpReq.remoteHost().toString());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_HOST_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_LOCAL_ADDR_ATTRIBUTE) {
			// req.localAddr().setString(httpReq.localAddr().toString());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_LOCAL_ADDR_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_REMOTEPORT_ATTRIBUTE) {
			// req.setRemotePort(httpReq.getRemotePort());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_REMOTEPORT_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_LOCALPORT_ATTRIBUTE) {
			// req.setLocalPort(httpReq.getLocalPort());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_LOCALPORT_ATTRIBUTE");
			}
		} else if (actionCode == ActionCode.ACTION_REQ_SSL_ATTRIBUTE) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_SSL_ATTRIBUTE");
			}
			throw new UnsupportedOperationException("SSL not supported");
		} else if (actionCode == ActionCode.ACTION_REQ_SSL_CERTIFICATE) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_SSL_CERTIFICATE");
			}
			throw new UnsupportedOperationException("SSL not supported");
		} else if (actionCode == ActionCode.ACTION_REQ_SET_BODY_REPLAY) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_REQ_SET_BODY_REPLAY");
			}
			// TODO what does this do?

		} else if (actionCode == ActionCode.ACTION_AVAILABLE) {
			// req.setAvailable(httpReq.getBody().available());
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_AVAILABLE");
			}
			// TODO 1是随便设的，要调查清楚这个参数是干啥的
			coyoteRequst.setAvailable(1);
		} else if (actionCode == ActionCode.ACTION_COMET_BEGIN) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_COMET_BEGIN");
			}
		} else if (actionCode == ActionCode.ACTION_COMET_END) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_COMET_END");
			}
		} else if (actionCode == ActionCode.ACTION_COMET_CLOSE) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_COMET_CLOSE");
			}
		} else if (actionCode == ActionCode.ACTION_COMET_SETTIMEOUT) {
			if (logger.isTraceEnabled()) {
				logger.trace("ActionCode.ACTION_COMET_SETTIMEOUT");
			}
		} else {
			logger.warn("Unhandled ActionCode:" + actionCode.getCode());
		}
	}
}
