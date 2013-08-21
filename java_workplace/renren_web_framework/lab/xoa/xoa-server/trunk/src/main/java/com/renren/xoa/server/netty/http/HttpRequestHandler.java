package com.renren.xoa.server.netty.http;

import java.util.concurrent.Executor;

import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.HttpRequest;

import com.renren.xoa.server.ServerContext;
import com.renren.xoa.server.XoaServerComponent;
import com.renren.xoa.server.coyote.NettyRequestAdapter;
import com.renren.xoa.server.monitor.ServerMonitor;

/**
 * 只处理MessageEvent中的message是{@link HttpRequest}的情况。
 * 将拿到的HttpRequest交给工作线程池处理。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-2 上午11:12:21
 */
@ChannelPipelineCoverage("all")
public class HttpRequestHandler extends SimpleChannelUpstreamHandler
		implements XoaServerComponent {

	private ServerContext serverContext;
	
	private Executor executor;
	
	/**
     * 服务器监控
     */
    private ServerMonitor monitor = ServerMonitor.getInstance();
	
	@Override
	public void messageReceived(ChannelHandlerContext ctx, MessageEvent event)
			throws Exception {
		if (event.getMessage() instanceof HttpRequest) {
			
			monitor.incRequestReceived();
			executor.execute(new HttpRequestProcessor(event, serverContext));
			monitor.incRequestQueued();
			//WorkerThreadPool.getInstance().handle(event, serverContext);
		} else {
            super.messageReceived(ctx, event);
        }
	} 

	private class HttpRequestProcessor implements Runnable {

		private MessageEvent event;
		
		private ServerContext serverContext;
		
		public HttpRequestProcessor(MessageEvent event, ServerContext serverContext) {
			this.event = event;
			this.serverContext = serverContext;
		}
		
		@Override
		public void run() {
			monitor.descRequestQueued();
			monitor.incWorkingThread();
			try {
				Channel channel = event.getChannel();
				if (channel != null && channel.isOpen()) {
					NettyRequestAdapter reqAdapter = 
						NettyRequestAdapter.construct(event, serverContext);
					serverContext.getProtocolHandlerAdapter()
							.coyoteService(reqAdapter.getCoyoteRequst(),
									reqAdapter.getCoyoteResponse());
				}
			} finally {
				monitor.descWorkingThread();
				monitor.incResponseSent();
			}
		}
	}
	
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e)
			throws Exception {
		e.getCause().printStackTrace();
		//e.getChannel().close();
	}

	@Override
	public void init(ServerContext serverContext) {
		this.serverContext = serverContext;
		this.executor = serverContext.getExecutor();
	}
}
