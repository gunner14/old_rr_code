package com.renren.xoa.proxy.client;

import static org.jboss.netty.channel.Channels.pipeline;

import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;

import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameDecoder;
import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameEncoder;

public class ClientPipelineFactory implements ChannelPipelineFactory {
	
	private ChannelHandler handler = null;
	
	public ClientPipelineFactory(ChannelHandler handler) {
		this.handler = handler;
	}
	
	@Override
	public ChannelPipeline getPipeline() throws Exception {
        
		System.out.println(this.getClass().getName() + ".getPipeline() is invoked");
		
		// Create a default pipeline implementation.
        ChannelPipeline pipeline = pipeline();

        //downStream
        pipeline.addLast("frameEncoder", new SPDYFrameEncoder());
        
        //upStream
        pipeline.addLast("frameDecoder", new SPDYFrameDecoder().setForProxy(true));
        
        pipeline.addLast("xxx", handler);
        
        return pipeline;
    }
}