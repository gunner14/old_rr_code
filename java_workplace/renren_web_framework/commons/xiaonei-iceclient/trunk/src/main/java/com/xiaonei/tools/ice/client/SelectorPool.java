package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicInteger;

public class SelectorPool {
	private Selector[] readerSelectors;
	private Selector[] writerSelectors;
	private AtomicInteger readerNext = new AtomicInteger();
	private AtomicInteger writerNext = new AtomicInteger();
	public SelectorPool(final int readSize,final int writeSize){
		writerSelectors = new Selector[writeSize];
		for(int i= 0 ;i<writeSize;i++){
			try {
				Selector selector = new WriterSelector();
				writerSelectors[i] = selector;
				new Thread(selector).start();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
		
		readerSelectors = new Selector[readSize];
		for(int i=0;i< readSize ; i++){
			try {
				Selector selector = new ReaderSelector();
				readerSelectors[i] = selector;
				new Thread(selector).start();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	public Selector getReaderSelector(){
		int next  = readerNext.incrementAndGet();
		Selector selector = readerSelectors[next % readerSelectors.length];
		return selector;
	}
	public Selector getWriterSelector(){
		int next  = writerNext.incrementAndGet();
		Selector selector = writerSelectors[next % writerSelectors.length];
		return selector;
	}
}
