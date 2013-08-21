package com.renren.commons.tools.holmes.mxbean;

import java.io.IOException;
import java.lang.management.ClassLoadingMXBean;
import java.lang.management.ManagementFactory;

import javax.management.MBeanServerConnection;

public class ClassLoadingInfo {

    public static void printClassLoadInfo(final MBeanServerConnection conn) {
        ClassLoadingMXBean clMBean = null;
        try {
            clMBean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.CLASS_LOADING_MXBEAN_NAME, ClassLoadingMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("Total Loaded Class Count:" + clMBean.getTotalLoadedClassCount());
        System.out.println("Loaded Class Count:" + clMBean.getLoadedClassCount());
        System.out.println("Unloaded Class Count:" + clMBean.getUnloadedClassCount());
        System.out.println("isVerbose:" + clMBean.isVerbose());
        System.out.println();
        //here we can turn on verbose of class loading dynamically
        //clMBean.setVerbose(verbose);
    }

}
