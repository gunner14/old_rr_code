package com.xiaonei.tools.ice.client.test;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.junit.Test;

import com.xiaonei.tools.ice.client.ice.Identity;


public class IceClient {
	@Test
	public void all(){
		com.xiaonei.tools.ice.client.IceClient client = new com.xiaonei.tools.ice.client.IceClient(new java.net.InetSocketAddress("127.0.0.1",10000),1,1,1);
		Identity echoId = new Identity("echo","");
		EchoOperation op = new EchoOperation(echoId,"echo");
		client.addOperation(op);
		ByteBuffer data = op.getData();
		System.out.println("value length="+data.limit());
		op = new EchoOperation(echoId,"echo");
		client.addOperation(op);
		op.getData();
	}
	@Test
	public void mina(){
		com.xiaonei.tools.ice.client.IceClient client = new com.xiaonei.tools.ice.client.IceClient(new java.net.InetSocketAddress("127.0.0.1",9999),1,1,1);
		Identity echoId = new Identity("echo","");
		EchoOperation op = new EchoOperation(echoId,"echo");
		client.addOperation(op);
		ByteBuffer data = op.getData();
		System.out.println("op1");
		op = new EchoOperation(echoId,"echo");
		client.addOperation(op);
		op.getData();
		System.out.println("op1");
	}
	@Test
	public void getIdentity(){
		System.out.println(Ice.Util.stringToIdentity("echo").category);
	}
	@Test
	public void selector() throws IOException{
		java.nio.channels.Selector selector = java.nio.channels.Selector.open();
		selector.select();
		System.out.println("selector over");
	}
}
