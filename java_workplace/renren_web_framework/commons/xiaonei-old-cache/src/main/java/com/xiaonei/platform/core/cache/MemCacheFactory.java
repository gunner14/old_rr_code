package com.xiaonei.platform.core.cache;

import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

import com.xiaonei.platform.core.cache.MemCacheManager;

public class MemCacheFactory{
	
	protected synchronized void update(String lastTime){
		Iterator<MemCacheManager> iter = managers.values().iterator();
		while(iter.hasNext()){
			MemCacheManager man = iter.next();
			man.update(lastTime);
		}
	}
	protected synchronized void checkPoolInit(){
		Iterator<MemCacheManager> iter = managers.values().iterator();
		while(iter.hasNext()){
			MemCacheManager man = iter.next();
			man.checkPoolInit();
		}
	}
	
	private java.util.concurrent.ConcurrentHashMap<String,MemCacheManager> managers = new ConcurrentHashMap<String,MemCacheManager>();
	private static MemCacheFactory instance = new MemCacheFactory();
	private MemUpdateMonitor monitor;
	private MemCacheFactory(){
		System.setProperty("net.spy.log.LoggerImpl", "net.spy.memcached.compat.log.Log4JLogger");
		monitor = new MemUpdateMonitor();
		monitor.start();
	}
	
	public void stopMonitor(){
		monitor.stopMonitor();
	}
	/**
	 * 单例方法
	 * @return
	 */
	public static MemCacheFactory getInstance(){
		return instance;
	}
	/**
	 * 通过名字获取缓存池
	 * 为防止并发，暂时使用双重检测
	 * @param poolName
	 * @return
	 */
	public MemCacheManager getManagerByPoolName(MemCacheKeys pool){
		if(pool==null)throw new IllegalArgumentException();
		String poolName = pool.getPoolName();
		MemCacheManager manager = managers.get(poolName);
		if(manager==null){
			manager = new MemCacheManager(poolName);
			MemCacheManager old = managers.putIfAbsent(poolName, manager);
			if(old!=null){
				return old;
			}
			manager.initPool(poolName,true);//设置Sock Pool
			System.out.println("init pool "+poolName+" over");
		}
		return manager;
	}
}