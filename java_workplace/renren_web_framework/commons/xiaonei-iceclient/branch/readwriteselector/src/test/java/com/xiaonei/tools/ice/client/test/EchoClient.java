package com.xiaonei.tools.ice.client.test;

import com.xiaonei.tools.ice.client.ice.Identity;

public class EchoClient {
	public static void main(String[] args){
		com.xiaonei.tools.ice.client.IceClient client = new com.xiaonei.tools.ice.client.IceClient(new java.net.InetSocketAddress("10.3.19.9",12345),1,1);
		Identity echoId = new Identity("M","");
		
		EchoOperation op = new EchoOperation(echoId,"echo");
		client.addOperation(op);
		op.getData();
		System.out.println("over");
	}
}
