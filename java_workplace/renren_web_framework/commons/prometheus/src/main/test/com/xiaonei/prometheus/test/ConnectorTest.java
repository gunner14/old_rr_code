package com.xiaonei.prometheus.test;

import java.net.InetSocketAddress;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.xiaonei.prometheus.nio.Buffer;
import com.xiaonei.prometheus.nio.CoreService;
import com.xiaonei.prometheus.nio.Option;
import com.xiaonei.prometheus.server.ConnectionHandler;
import com.xiaonei.prometheus.server.DefaultMessageHandler;
import com.xiaonei.prometheus.server.Message;
import com.xiaonei.prometheus.server.Protocol;
import com.xiaonei.prometheus.server.ProtocolHandlerFactory;

public class ConnectorTest {
	CoreService core;
	InetSocketAddress address = new InetSocketAddress("127.0.0.1",18888);
	@Before
	public void init(){
		core = new CoreService(new Option(4));
		core.setProtoclHandlerFactory(new ProtocolHandlerFactory());
		core.addMessageHandler(new DefaultMessageHandler());
		core.bind(address);
	}
	
	@Test
	public void test(){
		ConnectionHandler connection = new ConnectionHandler();
		core.connect(address,connection);
		
		for(int i=1;i<10000;i++){
			Buffer buffer = new Buffer();
			String args = "prometheus come to renren.com";
			buffer.writeByte(args.getBytes());
			
			Message send = new Message();
			send.setSeqId(i);
			send.setOpcode(Protocol.OPCODE_GET);
			send.setData(buffer);
			
			Message data = connection.syncWrite(send);
			Assert.assertTrue(data!=null);
			Assert.assertTrue(data.getSeqId()==i);
			Assert.assertTrue(data.getOpcode() == send.getOpcode());
		}
	}
}