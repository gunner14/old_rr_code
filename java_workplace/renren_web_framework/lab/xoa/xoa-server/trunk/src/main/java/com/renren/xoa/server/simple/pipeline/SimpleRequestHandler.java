package com.renren.xoa.server.simple.pipeline;

import java.util.concurrent.Executor;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.HttpResponseStatus;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.renren.xoa.commons.netty.SpdyHttpRequest;
import com.renren.xoa.commons.netty.SpdyHttpResponse;
import com.renren.xoa.server.monitor.ServerMonitor;
import com.renren.xoa.server.simple.SimpleXoaServer;


/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-1 下午04:31:58
 */
@ChannelPipelineCoverage("all")
public class SimpleRequestHandler extends SimpleChannelUpstreamHandler{

	private ServerMonitor monitor = ServerMonitor.getInstance();
	
	private Executor executor;
	
	private Executor[] executors = new Executor[8];
	
	private int index = 0;
	
	public SimpleRequestHandler() {
		for (int i = 0; i < executors.length; i++) {
			executors[i] = SimpleXoaServer.getExecutor();
		}
		
	}
	
	public SimpleRequestHandler setExecutor(Executor executor) {
		this.executor = executor;
		return this;
	}
	
	@Override
	public void messageReceived(ChannelHandlerContext ctx, final MessageEvent event)
			throws Exception {
		
		if (event.getMessage() instanceof SpdyHttpRequest) {
			if (executor != null) {
				monitor.incRequestReceived();
				executors[index++%8].execute(new Runnable() {
					@Override
					public void run() {
						monitor.incWorkingThread();
						handle(event);
						monitor.descWorkingThread();
						monitor.descRequestQueued();
					}
				});
				monitor.incRequestQueued();
			} else {
				handle(event);
			}
		} else {
			throw new NullPointerException();
		}
	}

	private void handle(MessageEvent event) {
		SpdyHttpRequest request = (SpdyHttpRequest)event.getMessage();
		int streamId = request.getStreamId();
		String uri = request.getUri();

		String[] ss = uri.split("/");
		
		String instruction = ss[1];
		int size = Integer.parseInt(ss[2]);
		
		if (instruction.equals("text")) {
			SpdyHttpResponse nettyResponse = new SpdyHttpResponse(HttpVersion.HTTP_1_1,
					HttpResponseStatus.valueOf(200),streamId);
			String responseContent = "not implemented";//TextFactory.getInstance().getString(size);
			
			ChannelBuffer buff;
			if (size == 0) {
				buff = ChannelBuffers.EMPTY_BUFFER;
			} else {
				buff = ChannelBuffers.dynamicBuffer(responseContent.length() + 10);
				buff.writeBytes(responseContent.getBytes());
			}
			nettyResponse.setContent(buff);
			event.getChannel().write(nettyResponse);
			
			monitor.incResponseSent();
		} else {
			System.err.println(uri);
		}
	}
	
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e)
			throws Exception {
		e.getCause().printStackTrace();
	}
}
