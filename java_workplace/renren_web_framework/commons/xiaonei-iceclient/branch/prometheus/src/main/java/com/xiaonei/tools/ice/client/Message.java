package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Message {
	private static final int COMPLETE = 1;
	private static final int HEADER = 2;
	private static final int BODY=3;
	
	private int state;
	private byte messageType;
	private int messageSize;
	private int readOffset;
	//private byte[] body;
	private ByteBuffer body;
	private ByteBuffer header = ByteBuffer.allocate(Protocol.headerSize);
	private int leftHeader = Protocol.headerSize;
	private int requestId ;
	//private ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
	public Message(){
		state = HEADER;
		header.order(ByteOrder.LITTLE_ENDIAN);
	}
	public int getState(){
		return state;
	}
	public boolean isComplete(){
		return state == COMPLETE;
	}
	public int getMessageType(){
		return messageType;
	}
	public int getMessageSize(){
		return messageSize;
	}
	public boolean read(ByteBuffer data) throws IOException{
		while(data.hasRemaining() && state!=COMPLETE){
			switch(state){
				case HEADER:
					if(readHeader(data)){
						//System.out.println("message read header ok");
						verifyHeader();
						//System.out.println("messageType = "+messageType+" messageSize = "+messageSize);
						if(messageType == Protocol.validateConnectionMsg){
							state = COMPLETE;
						}else if(messageType == Protocol.replyMsg||messageType == Protocol.requestMsg){
							//System.out.println("read body");
							body = ByteBuffer.allocate(messageSize);
							body.order(ByteOrder.LITTLE_ENDIAN);
							state = BODY;
						}
					}
					break;
				case BODY:
					//System.out.println("read body process");
					if(readBody(data)){
						//System.out.println("read body complete");
						state = COMPLETE;
					}
					break;
				default:
					throw new IOException("error message state");
			}
		}
		
		if(state == COMPLETE)return true;
		return false;
	}
	
	private boolean readHeader(ByteBuffer data){
		int len = Math.min(data.remaining(), leftHeader);
		byte b[] = new byte[len];
		data.get(b);
		header.put(b);
		leftHeader -= len;
		if(leftHeader == 0){
			return true;
		}
		return false;
	}
	
	private boolean readBody(ByteBuffer data){
		int leftBody = messageSize - Protocol.headerSize - readOffset;
		int toRead = Math.min(data.remaining(), leftBody);
		byte b[] = new byte[toRead];
		data.get(b);
		body.put(b);
		readOffset += toRead;
		//System.out.println("readOffset "+readOffset);
		if(readOffset == messageSize - Protocol.headerSize){
			body.flip();
			requestId = body.getInt();
			body.position(0);
			return true;
		}
		return false;
	}
	public ByteBuffer getData(){
		return body;
	}
	public int getRequestId(){
		return requestId;
	}
	private void verifyHeader() throws IOException{
		header.flip();
		
		if(header.get()!=Protocol.magic[0]||header.get()!=Protocol.magic[1]||header.get()!=Protocol.magic[2]||header.get()!=Protocol.magic[3]){
			throw new IOException("BadMagicException");
		}
		if(header.get()!=Protocol.protocolMajor || header.get() > Protocol.protocolMinor){
			throw new IOException("UnsupportedProtocolException");
		}
		if(header.get()!=Protocol.encodingMajor || header.get() > Protocol.encodingMinor){
			throw new IOException("UnsupportedEncodingException");
		}
		
		messageType = header.get();//messageType
		byte compress = header.get();
		messageSize = header.getInt();
	}
}
