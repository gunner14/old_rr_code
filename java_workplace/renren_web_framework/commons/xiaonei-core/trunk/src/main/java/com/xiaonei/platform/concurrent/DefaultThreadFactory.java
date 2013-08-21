package com.xiaonei.platform.concurrent;

import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 默认的ThreadFactory实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-9-19 下午08:05:18
 */
public class DefaultThreadFactory implements ThreadFactory {
	
	private static final AtomicInteger poolNumber = new AtomicInteger(1);
	private final ThreadGroup group;
    private final AtomicInteger threadNumber = new AtomicInteger(1);
    private final String namePrefix;
    
    /**
     * @param threadUsage 这个{@link ThreadFactory}创建的Thread的用途描述，
     * 用于区分不同用途的线程，如XXXReloader或XXXTimer等。
     */
    public DefaultThreadFactory(String threadUsage) {
        SecurityManager s = System.getSecurityManager();
        group = (s != null)? s.getThreadGroup() :
                             Thread.currentThread().getThreadGroup();
        namePrefix = threadUsage + "-" +
                      poolNumber.getAndIncrement() +
                     "-thread-";
    }

    /* (non-Javadoc)
     * @see java.util.concurrent.ThreadFactory#newThread(java.lang.Runnable)
     */
    @Override
    public Thread newThread(Runnable r) {
        Thread t = new Thread(group, r,
                              namePrefix + threadNumber.getAndIncrement(),
                              0);
        if (t.getPriority() != Thread.NORM_PRIORITY)
            t.setPriority(Thread.NORM_PRIORITY);
        t.setDaemon(true);
        return t;
    }
}
