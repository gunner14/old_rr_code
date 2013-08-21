package com.renren.xoa.proxy;

import org.jboss.netty.channel.ChannelHandlerContext;

import com.renren.xoa.commons.spdy.frame.Frame;

public interface FrameProxy {

	public void proxy(Frame frame, ChannelHandlerContext toClientCtx);
	
}
