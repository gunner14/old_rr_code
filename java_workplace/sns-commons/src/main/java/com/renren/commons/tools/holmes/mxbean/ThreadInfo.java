package com.renren.commons.tools.holmes.mxbean;

import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import javax.management.MBeanServerConnection;

public class ThreadInfo {

    public static void printThreadInfo(final MBeanServerConnection conn) {
        ThreadMXBean mbean = null;
        try {
            mbean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.THREAD_MXBEAN_NAME, ThreadMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("Dead locked Threads(处于死锁状态的线程):" + mbean.findDeadlockedThreads());
        System.out.println("Monitor Dead locked Threads(监视器处于死锁状态的线程):" + mbean.findMonitorDeadlockedThreads());
        System.out.println("Active Daemon Thread Count:" + mbean.getDaemonThreadCount());
        System.out.println("Active Peak Thread Count:" + mbean.getPeakThreadCount());
        System.out.println("Active Current Thread Count:" + mbean.getThreadCount());
        System.out.println("Total Started Thread Count:" + mbean.getTotalStartedThreadCount());
        System.out.println();
    }
}
