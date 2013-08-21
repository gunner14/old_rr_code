
package com.renren.xoa.server.threadpool;

import java.util.concurrent.Executor;

import org.apache.catalina.LifecycleException;
import org.apache.catalina.core.StandardThreadExecutor;
import org.apache.log4j.Logger;

/**
 * 在高并发情况下，Tomcat自带线程池StandardThreadExecutor中使用的任务队列
 * 实现LinkedBlockingQueue会成为一个瓶颈。
 * 
 * 这个实现是对其的一个优化，它内部封装了CPU数量个StandardThreadExecutor，也即相当于有
 * CPU数量TaskQueue，能够缓解竞争条件，但是缺陷是CPU利用率过高了。慎用！
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-4-8 下午03:33:05
 * 
 * @deprecated 实际上的可用性不强
 */
public class XoaThreadExecutor implements Executor {
    
	protected Logger logger = Logger.getLogger(this.getClass());
	
    protected int threadPriority = Thread.NORM_PRIORITY;

    protected boolean daemon = true;
    
    protected String namePrefix = "xoa-exec";
    
    protected int maxThreads = 100;
    
    protected int minSpareThreads = 5;
    
    protected int maxIdleTime = 60000;

    protected Executor[] executors;
    
    protected int cpus = 8;
    
    private int index = 0;
    
    public XoaThreadExecutor() {
    	cpus = Runtime.getRuntime().availableProcessors();
    	if (logger.isInfoEnabled()) {
    		logger.info("start XoaThreadExecutor with " + cpus + " inner executors");
    	}
    	executors = new Executor[cpus];
    	for (int i = 0; i < executors.length; i++) {
    		executors[i] = getExecutor(i);
		}
    }
    
    private Executor getExecutor(int index) {
		StandardThreadExecutor executor = new StandardThreadExecutor();
		executor.setDaemon(true);
		executor.setMaxThreads(maxThreads);
		executor.setMinSpareThreads(minSpareThreads);
		executor.setMaxIdleTime(maxIdleTime);
		executor.setNamePrefix(namePrefix + index + "-");
		return executor;
	}
    
    public void start() {
    	for (Executor exec : executors) {
    		if (exec instanceof org.apache.catalina.Executor) {
    			try {
					((org.apache.catalina.Executor) exec).start();
				} catch (LifecycleException e) {
					e.printStackTrace();
				}
    		}
    	}
    }
    
	@Override
	public void execute(Runnable command) {
		executors[index++ % cpus].execute(command);
		if (index > 50000000) {	//到一个很大的数就回滚一下
			index = 0;
		}
	}
	
	public void stop() {
		for (Executor exec : executors) {
    		if (exec instanceof org.apache.catalina.Executor) {
    			try {
					((org.apache.catalina.Executor) exec).stop();
				} catch (LifecycleException e) {
					e.printStackTrace();
				}
    		}
    	}
	}
}
