package com.xiaonei.tools.ice.client;

import java.nio.ByteBuffer;

import com.xiaonei.tools.ice.client.ice.BasicStream;
import com.xiaonei.tools.ice.client.ice.Identity;

public class BaseOperation implements Operation{
	private int requestId;
	protected BasicStream stream;
	private ByteBuffer data;//value
	private java.util.concurrent.CountDownLatch latch;
	public BaseOperation(Identity identity,String operation){
		latch = new java.util.concurrent.CountDownLatch(1);
		stream =  new BasicStream();
		writeHeader(identity,operation);
	}
	private void writeHeader(Identity identity,String operation){
		stream.writeBlob(Protocol.requestHdr);
		identity.__write(stream);
		stream.writeStringSeq(null);//fact
		stream.writeString(operation);//operation
		stream.writeByte((byte)0);//mode
		stream.writeSize(0);//context;
		stream.startWriteEncaps();
	}
	
	@Override
	public ByteBuffer getBuffer() {
		return stream.getBuffer().b;
	}

	@Override
	public int getRequestId() {
		return requestId;
	}

	@Override
	public void setRequestId(int requestId) {
		this.requestId = requestId;
		stream.pos(Protocol.headerSize);
		stream.writeInt(requestId);
		stream.pos(0);
	}

	@Override
	public void writeComplete() {
		//System.out.println("requestId "+requestId+" write ok");
	}
	
	
	public String toString(){
		return "requestId="+requestId;
	}
	
	public ByteBuffer getData(){
		if(data!=null)return data;
		try {
			latch.await();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return data;
	}
	
	public void callback(ByteBuffer data){
		this.data = data;
		latch.countDown();
	}
}
