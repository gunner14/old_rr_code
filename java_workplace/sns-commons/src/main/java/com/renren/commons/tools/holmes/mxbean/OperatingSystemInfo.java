package com.renren.commons.tools.holmes.mxbean;

import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;

import javax.management.MBeanServerConnection;

public class OperatingSystemInfo {

    public static void printOperatingSystemInfo(final MBeanServerConnection conn) {
        OperatingSystemMXBean mbean = null;
        try {
            mbean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.OPERATING_SYSTEM_MXBEAN_NAME, OperatingSystemMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("Operating System Name:" + mbean.getName());
        System.out.println("Operating System Version:" + mbean.getVersion());
        System.out.println("Operating System Arch:" + mbean.getArch());
        System.out.println("Operating System Available Processors:" + mbean.getAvailableProcessors());
        System.out.println();
    }
}
