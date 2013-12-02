package com.renren.xoa2.quota;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 计数器工厂
 * @author Xue Wenxin Aug 24, 2012 5:52:47 PM
 */
public class CounterFactory {
	
	private static Log logger = LogFactory.getLog(CounterFactory.class);
	
	/** 计数器到本地缓存*/
	private Map<String, Counter> counters = new ConcurrentHashMap<String, Counter>();
	
	private CounterFactory(){
		
	}
	private static CounterFactory instance = new CounterFactory();
	
	public static CounterFactory getInstance(){
		return instance;
	}
	//TODO：先这么写，等checker里面到那几个变成抽取成对象后再改
	public Counter getCounter(String key){
		Counter counter = counters.get(key);
		if(counter == null){
			counter = new Counter();
		}
		counters.put(key, counter);
		return counter;
	}
	
	
}
