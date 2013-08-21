/**
 * 
 */
package com.xiaonei.click.pipeline;

import static org.jboss.netty.channel.Channels.pipeline;

import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.handler.codec.http.HttpRequestDecoder;
import org.jboss.netty.handler.codec.http.HttpResponseEncoder;
import org.jboss.netty.handler.timeout.ReadTimeoutHandler;
import org.jboss.netty.util.HashedWheelTimer;
import org.jboss.netty.util.Timer;

import com.xiaonei.click.configure.Config;

/**
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class HttpServerPipelineFactory implements ChannelPipelineFactory {

    private Class<? extends ChannelHandler> handlerClass = null;

    Timer timer = new HashedWheelTimer();

    public HttpServerPipelineFactory(Class<? extends ChannelHandler> handlerClass) {
        this.handlerClass = handlerClass;
    }

    @Override
    public ChannelPipeline getPipeline() throws Exception {
        ChannelPipeline pipeline = pipeline();
        pipeline.addLast("decoder", new HttpRequestDecoder());
        pipeline.addLast("encoder", new HttpResponseEncoder());
        pipeline.addLast("timeout", new ReadTimeoutHandler(timer, Integer.parseInt(Config
                .getInstance().get(Config.Key.KEEP_ALIVE, null).toString())));
        pipeline.addLast("nocontent", handlerClass.newInstance());
        return pipeline;
    }

}
