package com.xiaonei.prometheus.nio;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectableChannel;
import java.nio.channels.SelectionKey;
import java.util.Set;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Selector extends Thread{
	private java.nio.channels.Selector selector;
	private java.util.concurrent.ConcurrentLinkedQueue<SelectorContext> queue;
	public Selector(){
		queue = new ConcurrentLinkedQueue<SelectorContext>();
		try {
			selector = java.nio.channels.Selector.open();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void register(java.nio.channels.SelectableChannel channel,int ops,EventHandler handler){
		queue.add(new SelectorContext(channel,ops,handler));
		selector.wakeup();
	}
	
	private void handlerQueue(){
		while(!queue.isEmpty()){
			SelectorContext context = queue.poll();
			try {
				context.channel.register(selector, context.ops,context.handler);
			} catch (ClosedChannelException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public void run(){
		while(true){
			handlerQueue();
			try{
				selector.select();
			}catch(IOException ex){
				//TODO handler exception
			}
			
			Set<SelectionKey> keys = selector.selectedKeys();
			if(keys.isEmpty())continue;
			for(SelectionKey key:keys){
				EventHandler handler = (EventHandler)key.attachment();
				if(handler==null)continue;
				try {
					handler.doHandler(key);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					key.cancel();
					try {
						key.channel().close();
					} catch (IOException e1) {
						e1.printStackTrace();
					}
				}
			}
			
			keys.clear();
		}
	}
	
	private static class SelectorContext{
		public final SelectableChannel channel;
		public final int ops;
		public final EventHandler handler;
		public SelectorContext(SelectableChannel channel,int ops,EventHandler handler){
			this.channel = channel;
			this.ops = ops;
			this.handler = handler;
		}
	}
}
