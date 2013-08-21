package com.renren.xoa.server.netty;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

/**
 * 用来监测Upstream上的连接状态，用log记录channelConnected和
 * channelDisconnected这两个事件。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-4-19 下午05:29:41
 */
@ChannelPipelineCoverage("all")
public class ConnectionStatusListener extends SimpleChannelUpstreamHandler {

    protected static Log logger = LogFactory.getLog(ConnectionStatusListener.class);

    @Override
    public void channelConnected(ChannelHandlerContext ctx, ChannelStateEvent e) throws Exception {
        StringBuilder msg = new StringBuilder(128);
        msg.append("XOA-Client connected: ");
        msg.append(e.getChannel().getRemoteAddress());
        //System.out.println(msg);
        if(logger.isInfoEnabled()) {
            logger.info(msg);
        }
        super.channelConnected(ctx, e);
    }

    @Override
    public void channelDisconnected(ChannelHandlerContext ctx, ChannelStateEvent e)
            throws Exception {
        StringBuilder msg = new StringBuilder(128);
        msg.append("XOA-Client disconnected: ");
        msg.append(e.getChannel().getRemoteAddress());
        //System.out.println(msg);
        if(logger.isInfoEnabled()) {
            logger.info(msg);
        }
        super.channelDisconnected(ctx, e);
    }
    
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e) throws Exception {
        
        if (e.getCause() instanceof IOException
                && "Connection reset by peer".equals(e.getCause().getMessage())) {
            if (logger.isInfoEnabled()) {
                logger.info("Connection reset by peer: " + e.getChannel().getRemoteAddress());
            }
        } else {
            super.exceptionCaught(ctx, e);
        }
    }
}
