package com.renren.sns.guide.utils;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


/**
 * 线程池 <br/>
 * 异步操作，都应该委托这个线程池来做!
 */
public class ThreadPool {

    private ExecutorService defaultExecutorService = null;

    private static ThreadPool defaultInstance = null;

    private ThreadPool() {
    	defaultExecutorService = Executors.newFixedThreadPool(16);

    }

    public static ThreadPool defaultInstance() {
	if (defaultInstance == null) {
	    synchronized (ThreadPool.class) {
		if (defaultInstance == null) {
		    defaultInstance = new ThreadPool();
		}
	    }
	}
	return defaultInstance;
    }

    public void execute(Runnable runnable) {
    	defaultExecutorService.execute(runnable);
    }


}
