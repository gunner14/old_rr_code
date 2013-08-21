package com.renren.xoa.server.coyote;

import java.io.IOException;

import org.apache.coyote.ActionCode;
import org.apache.coyote.OutputBuffer;
import org.apache.coyote.Response;
import org.apache.log4j.Logger;
import org.apache.tomcat.util.buf.ByteChunk;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.handler.codec.http.HttpResponse;

import com.renren.xoa.server.netty.NettyResponseSender;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-12 下午06:14:12
 * @deprecated 使用{@link InternalSpdyWrapOutputBuffer}性能更高一些
 */
public class InternalSpdyOutputBuffer implements OutputBuffer {

	private static Logger logger = Logger.getLogger(InternalSpdyOutputBuffer.class);
	
	private NettyRequestAdapter adapter;
	
	public InternalSpdyOutputBuffer(NettyRequestAdapter adapter) {
		this.adapter = adapter;
	}
	
	private static final int BUFF_SIZE = 8192;

	private ChannelBuffer buff = ChannelBuffers.directBuffer(BUFF_SIZE);

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
			int offset = 0;

			if (logger.isDebugEnabled()) {
				logger.debug("initially, bchunk.getLength()=" + length + ", offset="
						+ offset + ", writableBytes="
						+ buff.writableBytes());
			}

			//要doWrite的字节数量大于buff的writableBytes，就把buff填满发出去，直到剩余空间够用为止
			while (length - offset > buff.writableBytes()) {
				
				int writableBytes = buff.writableBytes();
				
				if (logger.isDebugEnabled()) {
					logger.debug("before, offset=" + offset
							+ ", writableBytes=" + writableBytes);
				}
				if (logger.isDebugEnabled()) {
					logger.debug("write to buffer:" + writableBytes + "bytes");
				}
				
				//有一种特殊情况，就是buff之前已经被填满了但尚未发送（未发送是为了等FIN_FLAG），
				//也就是说buff.writableBytes() == 0，
				//此时实际写入buff 0 bytes，然后把buff发出去，下次循环再写入
				buff.writeBytes(bchunk.getBytes(), bchunk.getStart()
						+ offset, writableBytes);
				offset += writableBytes;	
				
				nettyResponse.setContent(buff);
				if (logger.isDebugEnabled()) {
					logger.debug("send netty response with "
							+ buff.readableBytes() + "bytes");
				}
				NettyResponseSender.send(messageEvent, nettyResponse, false);
				if (logger.isDebugEnabled()) {
					logger.debug("after, offset="
							+ offset + ", writableBytes="
							+ buff.writableBytes());
				}
			}

			// 把剩余字节buff，此时buff还有剩余空间，或者刚好写满，这时候不着急发送，等填满或等FIN_FLAG，
			// 即最后一次发送让ACTION_CLOSE来触发
			buff.writeBytes(bchunk.getBytes(), bchunk.getStart() + offset,
					length - offset);
			if (logger.isDebugEnabled()) {
				logger.debug("write to buffer:" + (length - offset) + "bytes");
			}
			
			nettyResponse.setContent(buff);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bchunk.getLength();
	}
}
