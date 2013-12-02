package com.renren.xoa2.quota.counter;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import com.renren.ad.datacenter.find.bean.QuotaInfo2.QuotaLevel;
import com.renren.xoa2.client.DefaultServiceFactory;
import com.renren.xoa2.client.IServiceFactory;
import com.renren.xoa2.quota.Counter;
import com.renren.xoa2.quota.cache.IQuotaCache;
import com.renren.xoa2.quota.cache.QuotaCacheConstants;
import com.renren.xoa2.quota.cache.QuotaCacheException;

/**
 * QuotaCounter Test Case
 * @author Xue Wenxin Sep 19, 2012 5:22:27 PM
 */
public class TestQuotaCounter {
	public static IServiceFactory factory = new DefaultServiceFactory();
	public static IQuotaCache cache = factory.getService(IQuotaCache.class);
	public static QuotaCounter counter = new DefaultQuotaCounter(cache);
	public static String key = "test-quota-counter";
	
	public static void sleep(long millis){
		try {
			Thread.sleep(millis);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	static class LoadRunner implements Runnable {
		private QuotaCounter counter = new DefaultQuotaCounter(cache);
        public void run() {
        	invoke();
        }
        private void invoke() {
        	for(int i = 0;i<100000;i++){
    			if(i%100==0){
    				sleep(50);
    			}
    			
    			long current = counter.increase(QuotaLevel.SEC, key);
//    			System.out.println(current);
    			if(current % 100 == 1){
    				System.out.println(Thread.currentThread().getName()+":"+current);
//    				System.out.println(counter.hashCode()+":"+current);
    			}
    		}
        }
    }
	
	/**
	 * a.测试total配额:多个线程代表多个client服务器，每个线程一个counter同时操作一个total_key
	 * b.测试single配额:多个线程代表多个调用请求，所有线程共用一个counter同时操作一个single_key 
	 */
	@Test
	public void testMultiThread(){
		long start = System.currentTimeMillis();
		List<Thread> threadList = new ArrayList<Thread>();
		for( int i=0; i<5; i++){
    		Thread thread = new Thread( new LoadRunner(), "loadRunner" + String.valueOf(i));
    		thread.start();
    		threadList.add(thread);
		}
		try {
        	for (Thread thread: threadList) {
                thread.join();        		
        	}
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
		System.out.println("total time:"+(System.currentTimeMillis()-start));
	}
	/**
	 * 单线程：一个counter单独计数(update cache)
	 */
	@Test
	public void test(){
		long start = System.currentTimeMillis();
		for(int i = 0;i<100000;i++){
			if(i%100==0){
				sleep(60);
			}
			System.out.println(counter.increase(QuotaLevel.SEC, key));

		}
		System.out.println("total time:"+(System.currentTimeMillis()-start));
	}
	
	@Test
	public void getCache() throws QuotaCacheException{
		System.out.println(this);
		String k2 = "SEC_test-quota-counter";
		String k = "HOUR_HOUR_twitter.demo.xoa.renren.com_1_api";
		System.out.println(cache.getLong(k2));
		}
	@Test
	public void testCache() throws QuotaCacheException{
		long current = 0;
		long result = 0;
		int number =1;
		String key = "SEC_test-quota-counter";
		cache.setLong(key, 0, QuotaCacheConstants.EXPIRE_SECOND*10);
		for(int i=0;i<number;i++){
			long start = System.currentTimeMillis();
			if(cache.getLong(key) == QuotaCacheConstants.ERROR_COUNT){
				//do nothing
			}else{
				if(current == 1){
					//do nothing
				}else{
					if(current < 100000l){
						result +=(System.currentTimeMillis()-start);
//						System.out.println(System.currentTimeMillis()-start);
					}
					
				}
			}
		}
		System.out.println(result/number);
	}
	@Test
	public void testSynchro(){
		final Counter counter = new Counter();
		final String key = "test";
		List<Thread> threadList = new ArrayList<Thread>();
		for( int i=0; i<50; i++){
    		Thread thread = new Thread( new Runnable() {
				
				@Override
				public void run() {
					for(int j=0;j<100;j++){
						counter.increaseCount(key);
						if(true){
							System.out.println(Thread.currentThread().getName()+":"+counter.getCount(key));
						}
					}
				}
			}, "loadRunner" + String.valueOf(i));
    		thread.start();
    		threadList.add(thread);
		}
//		for(int z=0;z<5;z++){
//			threadList.get(z).start();
//		}
		try {
        	for (Thread thread: threadList) {
                thread.join();        		
        	}
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
//		counter.increase(QuotaLevel.SEC, key);
	}
}
