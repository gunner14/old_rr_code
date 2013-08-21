package com.renren.xoa.proxy.pipeline;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

import com.renren.xoa.commons.spdy.datastructure.ExpireWheel;
import com.renren.xoa.commons.spdy.frame.DataFrame;
import com.renren.xoa.commons.spdy.frame.Frame;
import com.renren.xoa.commons.spdy.frame.control.SynStream;
import com.renren.xoa.proxy.BackendManager;
import com.renren.xoa.proxy.FrameProxy;
import com.renren.xoa.proxy.client.FrameProxyManager;

@ChannelPipelineCoverage("one")
public class XoaProxyHandler extends SimpleChannelUpstreamHandler{
	
	protected final Log logger = LogFactory.getLog(this.getClass());
	
	private ExpireWheel<FrameProxy> proxyMap = new ExpireWheel<FrameProxy>(
			ExpireWheel.CAPACITY_2P18, 2);
	
	@Override
	public void messageReceived(ChannelHandlerContext ctx, final MessageEvent event)
			throws Exception {
		
		Object message = event.getMessage();
		if (message instanceof Frame) {
			if (event.getMessage() instanceof SynStream) {
				SynStream synStream = (SynStream)event.getMessage();
				String aimHost = lookupAimHost(synStream);
				FrameProxy proxy = findProxy(aimHost);
				proxy.proxy(synStream, ctx);
				proxyMap.put(synStream.getStreamId(), proxy);
			} else if(message instanceof DataFrame) {
				DataFrame dataFrame = (DataFrame)message; 
				int streamId = dataFrame.getStreamId();
				FrameProxy proxy = proxyMap.get(streamId);
				if (proxy != null) {
					proxy.proxy(dataFrame, ctx);
				} else {
					logger.error("No proxy for stream:" + streamId);					
				}
			} else {
				logger.warn("unexpected message received:" + message);
			}
		} else {
			logger.warn("unexpected message received:" + message);
			super.messageReceived(ctx, event);
		}
	}
	
	public String lookupAimHost(SynStream synStream) {
		return BackendManager.getInstance().getBackend();
		//return "127.0.0.1:8888";
	}
	
	public FrameProxy findProxy(String aimHost) {
		String[] ss = aimHost.split(":"); 
		String host = ss[0];
		int port = Integer.parseInt(ss[1]);
		//SpdyFrameClient proxy = new SpdyFrameClient(new InetSocketAddress(host, port));
		//proxy.init();
		FrameProxy proxy = FrameProxyManager.getInstance().getClient(aimHost);
		return proxy;
	}
	
	/*public FrameProxy doProxy(Frame frame, ChannelHandlerContext ctx) {
		FrameProxy proxy = null;
		proxy.proxy(frame);
		return proxy;
	}*/
}
