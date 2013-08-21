package com.xiaonei.prometheus.server;

import java.nio.ByteBuffer;
import org.apache.log4j.Logger;

import com.xiaonei.prometheus.LocalException;
import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.nio.IProtocolHandler;

/**
  *  非线程安全，需要在一个线程安全环境下运行，宿主在SessionHandler
 * @author zhangjie@opi-corp.com
 */
public class ProtocolHandler implements IProtocolHandler{
	private static final int COMPLETE = 1;
	private static final int HEADER = 2;
	private static final int BODY=3;
	private int state;
	
	private int leftHeader = Protocol.headerSize;
	private Buffer buffer;
	
	private int messageSize;
	private int seqId;
	private byte opcode;
	
	private int readOffset;
	
	private Logger log = Logger.getLogger(PrometheusServer.class);
	
	public ProtocolHandler(){
		reset();
	}
	
	public Object decode(ByteBuffer data) {
		while(data.remaining() > 0 && state != COMPLETE){
			switch(state){
				case HEADER:
					if(readHeader(data)){
						verifyHeader();
						state = BODY;
					}
					break;
				case BODY:
					if(readBody(data)){
						if(log.isDebugEnabled())
						log.debug("receiver messageSize="+messageSize+",requestId="+seqId);
						state = COMPLETE;
					}
					break;
				default:
					throw new LocalException("error message state");
			}
		}
		
		if(state == COMPLETE){
			buffer.flip();
			Message message = new Message();
			message.setSeqId(seqId);
			message.setSize(messageSize);
			message.setData(buffer);
			message.setOpcode(opcode);
			reset();
			return message;
		}else{
			return null;
		}
	}
	
	private void reset(){
		this.state = HEADER;
		this.buffer = new Buffer();
		this.messageSize = 0 ;
		this.seqId = 0;
		this.opcode = 0;
		this.leftHeader = Protocol.headerSize;
		this.readOffset = 0;
	}
	private boolean readHeader(ByteBuffer data){
		int len = Math.min(data.remaining(), leftHeader);
		byte b[] = new byte[len];
		data.get(b);
		buffer.writeByte(b);
		leftHeader -= len;
		if(leftHeader == 0){
			return true;
		}
		return false;
	}
	
	private boolean readBody(ByteBuffer data){
		log.debug("message read body readOffset = "+readOffset);
		int leftBody = messageSize - Protocol.headerSize - readOffset;
		int toRead = Math.min(data.remaining(), leftBody);
		byte b[] = new byte[toRead];
		data.get(b);
		buffer.writeByte(b);
		readOffset += toRead;
		if(readOffset == messageSize - Protocol.headerSize){
			return true;
		}
		return false;
	}
	
	public Buffer encode(Object send) {
		Message message = (Message)send;
		Buffer buffer = new Buffer();
		buffer.writeByte(Protocol.magic);
		buffer.writeByte(message.getOpcode());
		buffer.writeInt(Protocol.headerSize+message.getData().size());
		buffer.writeInt(message.getSeqId());
		
		Buffer body = message.getData();
		body.flip();
		byte[] b = new byte[body.size()];
		body.getByteBuffer().get(b);
		buffer.writeByte(b);
		buffer.flip();
		return buffer;
	}
	
	private void verifyHeader(){
		buffer.position(0);
		if(buffer.readByte()!=Protocol.magic[0]||buffer.readByte()!=Protocol.magic[1]||buffer.readByte()!=Protocol.magic[2]){
			throw new LocalException("BadMagicException");
		}
		
		opcode = buffer.readByte();
		messageSize = buffer.readInt();
		seqId = buffer.readInt();
	
		buffer.reset();
		
		if(log.isDebugEnabled())
		log.debug("opcode="+opcode+":messagesize="+messageSize+":seqId="+seqId);
	}
}
