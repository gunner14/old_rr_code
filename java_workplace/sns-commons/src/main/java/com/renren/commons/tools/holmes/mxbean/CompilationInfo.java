package com.renren.commons.tools.holmes.mxbean;

import java.io.IOException;
import java.lang.management.CompilationMXBean;
import java.lang.management.ManagementFactory;

import javax.management.MBeanServerConnection;

public class CompilationInfo {

    public static void printCompilationInfo(final MBeanServerConnection conn) {
        CompilationMXBean clMBean = null;
        try {
            clMBean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.COMPILATION_MXBEAN_NAME, CompilationMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("Compilation name:" + clMBean.getName());
        System.out.println("Total Compilation Time :" + clMBean.getTotalCompilationTime() + "ms");
        System.out.println();
    }
}
