package com.xiaonei.prometheus.test;

import org.junit.Assert;
import org.junit.Test;

import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.server.Message;
import com.xiaonei.prometheus.server.Protocol;
import com.xiaonei.prometheus.server.ProtocolHandler;


public class ProtocolTest {
	@Test
	public void testBuffer(){
		
	}
	@Test
	public void test(){
		org.apache.log4j.BasicConfigurator.configure();
		
		Buffer buffer = new Buffer();
		String args = "prometheus come to renren.com";
		buffer.writeByte(args.getBytes());
		
		Message send = new Message();
		send.setSeqId(1);
		send.setOpcode(Protocol.OPCODE_GET);
		send.setData(buffer);
		
		int bodySize = buffer.size();
		Assert.assertTrue(bodySize == args.length());
		
		ProtocolHandler protocol = new ProtocolHandler();
		Buffer encode = protocol.encode(send);
		Assert.assertTrue(encode.size()==(Protocol.headerSize+bodySize));
		
		Message decode = (Message)protocol.decode(encode.getByteBuffer());
		Assert.assertTrue(decode.getOpcode() == send.getOpcode());
		Assert.assertTrue(decode.getSeqId() == send.getSeqId());
	}
}
