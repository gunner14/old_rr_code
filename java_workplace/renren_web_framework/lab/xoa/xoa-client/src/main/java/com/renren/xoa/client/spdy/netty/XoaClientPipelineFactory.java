package com.renren.xoa.client.spdy.netty;

import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;

import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameDecoder;
import com.renren.xoa.commons.spdy.codec.frame.SPDYFrameEncoder;
import com.renren.xoa.commons.spdy.codec.http.FrameRequestEncoder;
import com.renren.xoa.commons.spdy.codec.http.MultiFrameResponseDecoder;

/**
 * Client端的{@link ChannelPipelineFactory}
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public class XoaClientPipelineFactory implements ChannelPipelineFactory {

    private ChannelHandler handler = null;

    public XoaClientPipelineFactory(ChannelHandler handler) {
        this.handler = handler;
    }

    @Override
    public ChannelPipeline getPipeline() throws Exception {
        // Create a default pipeline implementation.
        ChannelPipeline pipeline = Channels.pipeline();

        //downStream
        pipeline.addLast("frameEncoder", new SPDYFrameEncoder());
        pipeline.addLast("httpReqEncoder", new FrameRequestEncoder());
        //pipeline.addLast("httpRespEncoder", new FrameResponseEncoder());

        //upStream
        pipeline.addLast("frameDecoder", new SPDYFrameDecoder());
        //pipeline.addLast("httpReqDecoder", new FrameRequestDecoder());
        pipeline.addLast("httpRespDecoder", new MultiFrameResponseDecoder());
        pipeline.addLast("handler", handler);
        return pipeline;
    }

}
