package com.renren.commons.tools.holmes.mxbean;

import java.io.IOException;
import java.lang.management.ManagementFactory;
import java.lang.management.RuntimeMXBean;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.management.MBeanServerConnection;

public class RuntimeInfo {

    public static void printRuntimeInfo(final MBeanServerConnection conn) {
        RuntimeMXBean mbean = null;
        try {
            mbean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.RUNTIME_MXBEAN_NAME, RuntimeMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("VM Name:" + mbean.getVmName());
        System.out.println("VM Vendor:" + mbean.getVmVendor());
        System.out.println("VM Version:" + mbean.getVmVersion());
        final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        System.out.println("VM Start Time:" + sdf.format(new Date(mbean.getStartTime())));
        System.out.println();
    }
}
