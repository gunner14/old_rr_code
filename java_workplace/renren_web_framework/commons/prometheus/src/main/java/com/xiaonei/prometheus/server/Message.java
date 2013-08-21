package com.xiaonei.prometheus.server;

import com.xiaonei.prometheus.nio.Buffer;

public class Message{
	private int seqId;
	private int size;
	private byte opcode;
	private Buffer data;
	
	public Message(){
		
	}
	
	public void setSize(int size){
		this.size = size;
	}
	
	public void setSeqId(int seqId){
		this.seqId = seqId;
	}
	
	public void setData(Buffer data){
		this.data = data;
	}
	
	public void setOpcode(byte opcode){
		this.opcode = opcode;
	}
	
	public int getSeqId(){
		return seqId;
	}
	
	public int getSize(){
		return size;
	}
	
	public byte getOpcode(){
		return opcode;
	}
	
	public Buffer getData(){
		return data;
	}
}
