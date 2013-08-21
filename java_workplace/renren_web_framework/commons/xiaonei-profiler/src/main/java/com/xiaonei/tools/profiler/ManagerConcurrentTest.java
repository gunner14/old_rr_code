package com.xiaonei.tools.profiler;

import java.util.concurrent.CountDownLatch;

public class ManagerConcurrentTest implements Runnable {
	private CountDownLatch latch;
	public ManagerConcurrentTest(CountDownLatch latch){
		this.latch = latch;
	}
	public void run(){
		ProfilerManager manager = ProfilerManager.getInstanceByDomain("test.xiaonei.com");
		if(manager==null){
			System.out.println("null==========");
			return;
		}
		latch.countDown();
		//System.out.println("dd");
	}
	public static void main(String[] args){
		int n =  10000;
		CountDownLatch latch = new CountDownLatch(n);
		for(int i=0;i<n;i++){
			Thread t = new Thread(new ManagerConcurrentTest(latch));
			t.start();
		}
		try {
			latch.await();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("over");
	}
}
