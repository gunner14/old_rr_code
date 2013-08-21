package com.xiaonei.platform.core.cache;

import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.cache.MemCacheManager;

public class MemCacheFactory{
	
	private Log logger = LogFactory.getLog(this.getClass());
	
	/**
	 * 用来进行MemCacheManager初始化的线程池。
	 * 不使用同步线程来初始化的原因是：如果同步线程被上层调用者interrupt掉了，那么init过程中
	 * 建立的socket链接会被关闭，造成MemCacheManager进入一种异常的状态。
	 */
	private ExecutorService initWorker = Executors
			.newCachedThreadPool(new MyThreadFactory());

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
	public MemCacheManager getManagerByPoolName(MemCacheKeys pool) {
		if (pool == null)
			throw new IllegalArgumentException();
		final String poolName = pool.getPoolName();
		MemCacheManager manager = managers.get(poolName);
		if (manager == null) {
			
			//用poolName来上锁，避免使用全局锁
			synchronized (poolName.intern()) {
				manager = managers.get(poolName);
				if (manager == null) {
					
					long startTime = System.currentTimeMillis();
					
					manager = new MemCacheManager(poolName);
					
					//pool初始化的过程交给异步线程去做，不是为了省时间，是为了防止被interrupt
					final MemCacheManager tempMgr = manager;
					Future<?> future = initWorker.submit(new Runnable() {
						@Override
						public void run() {
							try {
								tempMgr.initPool(poolName, true);// 设置Sock Pool
							} catch (Throwable e) {
								e.printStackTrace();
							}
						}
					});
					
					//等待初始化完成
					try {
						future.get();
					} catch (InterruptedException e) {
						e.printStackTrace();
					} catch (ExecutionException e) {
						e.printStackTrace();
					}
					
					long endTime = System.currentTimeMillis();
					
					MemCacheManager old = managers.putIfAbsent(poolName,
							manager);
					if (old != null) {
						logger.error("memcached never reach here");
						return old;
					}
					long cost = endTime - startTime;
					if (cost >= 100) {
							logger.warn("SLOW->init pool " + poolName + " over, cost " + cost + "ms");
					} else {
						if (logger.isInfoEnabled()) {
							logger.info("init pool " + poolName + " over, cost " + cost + "ms");
						}
					}
				}
			}
		}
		return manager;
	}
}