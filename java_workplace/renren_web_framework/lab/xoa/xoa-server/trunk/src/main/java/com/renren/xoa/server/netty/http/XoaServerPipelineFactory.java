package com.renren.xoa.server.netty.http;

import static org.jboss.netty.channel.Channels.pipeline;

import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;

import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameDecoder;
import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameEncoder;
import com.renren.xoa.commons.spdy.codec.http.FrameRequestDecoder;
import com.renren.xoa.commons.spdy.codec.http.MultiFrameResponseEncoder;
import com.renren.xoa.server.ServerContext;
import com.renren.xoa.server.XoaServerComponent;
import com.renren.xoa.server.netty.ConnectionStatusListener;

/**
 * XoaServer的PipelineFactory
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-11 上午11:53:17
 */
public class XoaServerPipelineFactory implements ChannelPipelineFactory, XoaServerComponent {

	private HttpRequestHandler httpRequestHandler;
	
	//private ServerContext serverContext;
	
	@Override
	public ChannelPipeline getPipeline() throws Exception {
		
		// Create a default pipeline implementation.
        ChannelPipeline pipeline = pipeline();

        //downStream
        pipeline.addLast("frameEncoder", new SPDYFrameEncoder());
        pipeline.addLast("httpRespEncoder", new MultiFrameResponseEncoder());
        
        //upStream
        pipeline.addLast("connectionStatusListener", new ConnectionStatusListener());
        
        pipeline.addLast("frameDecoder", new SPDYFrameDecoder());
        pipeline.addLast("httpReqDecoder", new FrameRequestDecoder());
        
        pipeline.addLast("handler", httpRequestHandler);
        return pipeline;
    }

	@Override
	public void init(ServerContext serverContext) {
		httpRequestHandler = new HttpRequestHandler();
		httpRequestHandler.init(serverContext);
	}
}