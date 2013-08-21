package com.xiaonei.prometheus.server;

import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.atomic.AtomicInteger;

import com.xiaonei.prometheus.nio.MessageHandler;
import com.xiaonei.prometheus.nio.SessionHandler;

public class StatMessageHandler extends TimerTask implements MessageHandler{
	private int last;
	private volatile ThreadPoolExecutor workerThreadPool;
	@Override
	public void run() {
		int current = count.get();
		int tps = current-last;
		System.out.println(tps+" tps/s");
		last = current;
		if(workerThreadPool!=null){
			System.out.println("worker thread pool size = "+workerThreadPool.getPoolSize());
		}
	}
	private AtomicInteger count = new AtomicInteger(0);
	private Timer timer = new Timer();
	public StatMessageHandler(){
		this.timer.schedule(this, 10, 1000);
	}
	
	public void handle(SessionHandler session,Object message) {
		count.getAndIncrement();
		if(workerThreadPool==null){
			workerThreadPool = (ThreadPoolExecutor)session.getExecutorService();
		}
	}
}
