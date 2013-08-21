package com.renren.xoa.proxy.pipeline;

import static org.jboss.netty.channel.Channels.pipeline;

import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;

import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameDecoder;
import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameEncoder;

public class XoaProxyPipelineFactory implements ChannelPipelineFactory {

	
	
	@Override
	public ChannelPipeline getPipeline() throws Exception {
        
		System.out.println(this.getClass().getName() + ".getPipeline() is invoked");
		
		// Create a default pipeline implementation.
        ChannelPipeline pipeline = pipeline();

        //downStream
        pipeline.addLast("frameEncoder", new SPDYFrameEncoder());
        
        //upStream
        pipeline.addLast("frameDecoder", new SPDYFrameDecoder().setForProxy(true));
        pipeline.addLast("proxyHandler", new XoaProxyHandler());
        
        return pipeline;
    }
}