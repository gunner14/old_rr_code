package com.renren.xoa.server.netty;

import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.MessageEvent;

import com.renren.xoa.commons.netty.SpdyHttpResponse;

/**
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-12 下午06:21:49
 */
public class NettyResponseSender {

	public static void send(MessageEvent messageEvent, Object response, boolean close) {
		ChannelFuture future = messageEvent.getChannel().write(response);
		((SpdyHttpResponse)response).setCommited(true);	//第一次发送之后，就要设置为committed，后面再重复设置也无所谓
		// Close the connection after the write operation is done if necessary.
		if (close) {
			future.addListener(ChannelFutureListener.CLOSE);
		}
	}
	
}
