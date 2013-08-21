package com.xiaonei.page.xoa.task;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

public class AsynTaskExcutor {

	private static final Executor executor=Executors.newCachedThreadPool(new AsynThreadFactory());
	
	
	private static final AsynTaskExcutor instance=new AsynTaskExcutor();
	
	public final static AsynTaskExcutor getInstance(){
		return instance;
	}
	
	private AsynTaskExcutor(){}
	
	public void execute(final AsynTask task,final CallBack callBack){
		executor.execute(new Runnable() {
			
			@Override
			public void run() {
				task.process(callBack);
			}
		});
	}
	
	static class AsynThreadFactory implements ThreadFactory{
		
        static final AtomicInteger poolNumber = new AtomicInteger(1);
        final ThreadGroup group;
        final AtomicInteger threadNumber = new AtomicInteger(1);
        final String namePrefix;

        AsynThreadFactory() {
            SecurityManager s = System.getSecurityManager();
            group = (s != null)? s.getThreadGroup() :
                                 Thread.currentThread().getThreadGroup();
            namePrefix = "AsynThreadFactory-" +
                          poolNumber.getAndIncrement() +
                         "-thread-";
        }

        @Override
        public Thread newThread(Runnable r) {
            Thread t = new Thread(group, r,
                                  namePrefix + threadNumber.getAndIncrement(),
                                  0);
            if (t.isDaemon())
                t.setDaemon(false);
            if (t.getPriority() != Thread.NORM_PRIORITY)
                t.setPriority(Thread.NORM_PRIORITY);
            return t;
        }
		
	}
}
