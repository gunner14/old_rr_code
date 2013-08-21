package com.xiaonei.prometheus.test;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.concurrent.CountDownLatch;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.xiaonei.prometheus.nio.EventHandler;
import com.xiaonei.prometheus.nio.Selector;
import com.xiaonei.prometheus.nio.SelectorPool;


public class SelectorPoolTest {
	private SelectorPool selectorPool;
	private CountDownLatch latch;
	@Before
	public void init(){
		latch = new CountDownLatch(1);
		selectorPool = new SelectorPool(2);
		Assert.assertTrue(selectorPool!=null);
	}
	
	@Test
	public void testHttp() throws IOException{
		EventHandler handler = new EventHandler(){
			private ByteBuffer output;
			private ByteBuffer input;
			
			public void doHandler(SelectionKey key) throws IOException {
				if(!key.isValid())Assert.assertTrue(false);
				SocketChannel channel = (SocketChannel)key.channel();
				if(key.isConnectable()){
					Assert.assertTrue(true);
					channel.finishConnect();
					output = ByteBuffer.allocate(1024);
					output.order(ByteOrder.LITTLE_ENDIAN);
					String getCmd = "GET / HTTP/1.1\r\nHost:www.xiaonei.com\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.0.1) Gecko/2008070206 Firefox/3.0.1\r\nAccept-Charset: x-gbk,utf-8;q=0.7,*;q=0.7\r\n\r\n";
					output.put(getCmd.getBytes("UTF-8"));
					output.flip();
					key.interestOps(SelectionKey.OP_WRITE);
				}
				
				if(key.isWritable()){
					while(output.hasRemaining()){
						int n = channel.write(output);
						if(n==0){
							break;
						}
					}
					
					key.interestOps(SelectionKey.OP_READ);
					input = ByteBuffer.allocate(1024);
				}
				
				if(key.isReadable()){
					int n = channel.read(input);
					if(n==-1){
						Assert.assertTrue(false);
					}
					if(n>0){
						System.out.println(new String(input.array()));
						latch.countDown();
					}
					key.interestOps(0);
				}
			}
		};
		java.nio.channels.SocketChannel socket = java.nio.channels.SocketChannel.open();
		socket.configureBlocking(false);
		if(!socket.connect(new java.net.InetSocketAddress("www.xiaonei.com",80))){
			selectorPool.register(socket, SelectionKey.OP_CONNECT, handler);
		}
		
		try {
			latch.await();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
