package com.xiaonei.platform.core.control;

import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

class CutterThreadFactory implements ThreadFactory {

    final ThreadGroup group;

    final AtomicInteger threadNumber = new AtomicInteger(1);

    final String namePrefix;

    CutterThreadFactory() {
        SecurityManager s = System.getSecurityManager();
        group = (s != null) ? s.getThreadGroup() : Thread.currentThread().getThreadGroup();
        namePrefix = "Cutter.ThreadPool-thread-";
    }

    public Thread newThread(Runnable r) {
        Thread t = new Thread(group, r, namePrefix + threadNumber.getAndIncrement(), 0);
        if (t.isDaemon()) t.setDaemon(false);
        if (t.getPriority() != Thread.NORM_PRIORITY) t.setPriority(Thread.NORM_PRIORITY);
        return t;
    }

}
