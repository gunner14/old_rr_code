package com.renren.xoa2.quota;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 计数器，用于并发环境下的计数(以后如果需要将计数持久话，可以在这里扩展)
 * @author Xue Wenxin Aug 24, 2012 3:17:14 PM
 */
public class Counter {
	
	private static Log logger = LogFactory.getLog(Counter.class);
	
	private String name;
	
	private ConcurrentHashMap<String, AtomicInteger> countMap = null;
	
	public Counter(){
		countMap = new ConcurrentHashMap<String, AtomicInteger>();
	}
	
	public Counter(int concurrencyLevel){
		//设置concurrencyLevel
		countMap = new ConcurrentHashMap<String, AtomicInteger>(16,0.75f,concurrencyLevel);
	}
	
	public int increaseCount(String key){
		return increaseCount(key,1);
	}

	public void clearCount(String key){
		setCount(key, 0);
	}
	
	public int increaseCount(String key, int value) {
		AtomicInteger count = countMap.get(key);
		if(count == null){ //double check lock
			//Counter的一个 instance 的 name + key作为锁
			synchronized ((this.getClass().getName() + key).intern()) {
				count = countMap.get(key);
				if(count == null){	//
					count = new AtomicInteger();
					countMap.put(key, count);
				}
			}
		}
		return count.addAndGet(value);
	}
	
	/**
	 * 如果没有该key对应到counter，证明没有计数，所以返回0
	 * @param key
	 * @return
	 */
	public int getCount(String key){
		AtomicInteger count = countMap.get(key);
		if(count == null){
			logger.error("There is no Count for key ["+key+"]"+",Counter Instance is:"+this.getClass().getName());
			return 0;
		}
		return count.get();
	}
	
	public void setCount(String key, int value){
		AtomicInteger count = countMap.get(key);
		if(count == null){
			logger.error("There is no Count for key ["+key+"]"+",Counter Instance is:"+this.getClass().getName());
			return ;
		}
		count.set(value);
	}
	

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	
}
