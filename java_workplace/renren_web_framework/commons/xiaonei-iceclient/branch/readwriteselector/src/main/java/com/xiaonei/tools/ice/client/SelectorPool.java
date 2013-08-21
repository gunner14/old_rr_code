package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.nio.channels.SelectionKey;
import java.util.concurrent.atomic.AtomicInteger;

public class SelectorPool {
	private Selector[] selectors;
	private AtomicInteger selectorNext = new AtomicInteger();
	public SelectorPool(final int size){
		selectors = new Selector[size];
		for(int i=0;i< size ; i++){
			try {
				Selector selector = new Selector();
				selectors[i] = selector;
				new Thread(selector).start();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	public Selector getSelector(){
		int next  = selectorNext.incrementAndGet();
		Selector selector = selectors[next % selectors.length];
		return selector;
	}
//	public void register(Connection conn){
//		int next = selectorNext.getAndIncrement();
//		Selector selector = selectors[next % selectors.length];
//		selector.register(conn,SelectionKey.OP_READ);
//	}
}
