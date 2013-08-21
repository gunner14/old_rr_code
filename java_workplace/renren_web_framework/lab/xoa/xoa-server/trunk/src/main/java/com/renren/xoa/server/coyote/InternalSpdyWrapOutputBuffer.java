package com.renren.xoa.server.coyote;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.coyote.ActionCode;
import org.apache.coyote.OutputBuffer;
import org.apache.coyote.Response;
import org.apache.tomcat.util.buf.ByteChunk;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.handler.codec.http.HttpResponse;

import com.renren.xoa.server.netty.NettyResponseSender;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-12 下午06:14:12
 */
public class InternalSpdyWrapOutputBuffer implements OutputBuffer {

	private static Log logger = LogFactory.getLog(InternalSpdyWrapOutputBuffer.class);
	
	private NettyRequestAdapter adapter;
	
	public InternalSpdyWrapOutputBuffer(NettyRequestAdapter adapter) {
		this.adapter = adapter;
	}

	/**
	 * Write the contents of a byte chunk.
	 * 
	 * @param chunk
	 *            byte chunk
	 * @return number of bytes written
	 * @throws IOException
	 *             an undelying I/O error occured
	 */
	@Override
	public int doWrite(ByteChunk bchunk, Response response)
			throws IOException {

		if (logger.isTraceEnabled()) {
			logger.trace("doWrite " + bchunk.getLength() + " bytes:");
		}

		// 已经开始向response body中写出据，如果response还没有commit，就把它commit了
		if (!response.isCommitted()) {
			if (logger.isTraceEnabled()) {
				logger.trace("response not committed yet, commit it");
			}
			// Send the connector a request for commit. The connector should
			// then validate the headers, send them (using sendHeaders) and
			// set the filters accordingly.
			response.action(ActionCode.ACTION_COMMIT, null);
		}
		
		MessageEvent messageEvent = adapter.getMessageEvent();
		HttpResponse nettyResponse = adapter.getNettyResponse();
		// doWrite之前要确保SpdyHttpResponse已经初始化了，因为在某些特殊情况下，ACTION_ACK之后会直接doWrite
		// initSpdyHttpResponseIfNeed(response, bchunk.getLength());
		try {
			int length = bchunk.getLength();
			ChannelBuffer buff = ChannelBuffers.wrappedBuffer(bchunk.getBytes(), bchunk.getStart(), length);
			nettyResponse.setContent(buff);
			NettyResponseSender.send(messageEvent, nettyResponse, false);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bchunk.getLength();
	}
}
