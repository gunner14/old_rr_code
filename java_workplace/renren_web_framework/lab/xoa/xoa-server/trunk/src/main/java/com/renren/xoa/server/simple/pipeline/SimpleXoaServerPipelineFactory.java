package com.renren.xoa.server.simple.pipeline;

import static org.jboss.netty.channel.Channels.pipeline;

import java.util.concurrent.Executor;

import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;

import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameDecoder;
import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameEncoder;
import com.renren.xoa.commons.spdy.codec.http.FrameRequestDecoder;
import com.renren.xoa.commons.spdy.codec.http.FrameRequestEncoder;
import com.renren.xoa.commons.spdy.codec.http.FrameResponseDecoder;
import com.renren.xoa.commons.spdy.codec.http.FrameResponseEncoder;

/**
 * XoaServer的PipelineFactory
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-11 上午11:53:17
 */
public class SimpleXoaServerPipelineFactory implements ChannelPipelineFactory {
	
	private Executor executor;
	
	public SimpleXoaServerPipelineFactory setExecutor(Executor executor) {
		this.executor = executor;
		return this;
	}
	
	@Override
	public ChannelPipeline getPipeline() throws Exception {
        
		System.out.println(this.getClass().getName() + ".getPipeline() is invoked");
		
		// Create a default pipeline implementation.
        ChannelPipeline pipeline = pipeline();

        //downStream
        pipeline.addLast("frameEncoder", new SPDYFrameEncoder());
        pipeline.addLast("httpReqEncoder", new FrameRequestEncoder());
        pipeline.addLast("httpRespEncoder", new FrameResponseEncoder());
        //pipeline.addLast("encoder", new HttpResponseEncoder());
        
        //upStream
        pipeline.addLast("frameDecoder", new SPDYFrameDecoder());
        pipeline.addLast("httpReqDecoder", new FrameRequestDecoder());
        pipeline.addLast("httpRespDecoder", new FrameResponseDecoder());
        //pipeline.addLast("decoder", new HttpRequestDecoder());
        
        pipeline.addLast("httpRequestHandler", new SimpleRequestHandler().setExecutor(executor));
        
        return pipeline;
    }

}