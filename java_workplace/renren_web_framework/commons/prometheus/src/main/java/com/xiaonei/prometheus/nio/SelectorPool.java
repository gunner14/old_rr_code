package com.xiaonei.prometheus.nio;

import java.util.concurrent.atomic.AtomicInteger;

public class SelectorPool {
	private Selector[] pool;
	private AtomicInteger next = new AtomicInteger(0); 
	public SelectorPool(int size){
		this.pool = new Selector[size];
		for(int i = 0 ;i < size ; i++){
			pool[i] = new Selector();
			Thread thread = new Thread(pool[i]);
			thread.start();
		}
	}
	
	/**
	 * round robin
	 * @return
	 */
	private Selector getNextSelector(){
		int index = next.getAndIncrement();
		return pool[index%pool.length];
	}
	
	public void register(java.nio.channels.SelectableChannel fd,int ops,EventHandler handler){
		Selector selector = getNextSelector();
		selector.register(fd, ops,handler);
	}
}
