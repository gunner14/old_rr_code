package com.renren.xoa2.quota;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


/**
 * 模拟QuotaCheckerFactory，测试
 * @author Xue Wenxin Aug 15, 2012 9:13:17 PM
 */
public class TestFactory {
	private ConcurrentMap<String, Example> exampleCache = new ConcurrentHashMap<String, Example>();
	
	private static TestFactory instance = new TestFactory();
	
	public static TestFactory getInstance(){
		return instance;
	}
	
	public Example getExample(String number, int counter){
		Example ex = exampleCache.get(getKey(number,counter));
		if(ex == null){
			ex = new Example(number, counter);
			exampleCache.put(getKey(number,counter), ex);
		}
		return ex;
	}
	
	private String getKey(String number, int counter){
		return number+counter;
	}
	
	
	public static void main(String[] args) {
		TestFactory factory = TestFactory.getInstance();
		final Example ex1 = factory.getExample("example", 1);
		final Example ex2 = factory.getExample("example", 2);
		ScheduledThreadPoolExecutor timerExecutor = new ScheduledThreadPoolExecutor(1);
		timerExecutor.scheduleAtFixedRate(new Runnable() {
			
			public void run() {
				try {
	                System.out.println("ex1:"+ex1.getTemp());
	                System.out.println("ex2:"+ex2.getTemp());
	             
				} catch (Throwable e) {
				}
			}
		}, 1, 5, TimeUnit.SECONDS);
	}
}
