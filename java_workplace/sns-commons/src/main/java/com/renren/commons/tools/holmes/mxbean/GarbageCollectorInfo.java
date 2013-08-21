package com.renren.commons.tools.holmes.mxbean;

import java.lang.management.GarbageCollectorMXBean;
import java.lang.management.ManagementFactory;
import java.util.Set;

import javax.management.MBeanServerConnection;
import javax.management.ObjectName;

public class GarbageCollectorInfo {

    @SuppressWarnings("unchecked")
    public static void printGarbageCollectorInfo(final MBeanServerConnection conn) {
        try {
            final ObjectName gcName = new ObjectName(ManagementFactory.GARBAGE_COLLECTOR_MXBEAN_DOMAIN_TYPE + ",*");
            System.out.println();
            final Set names = conn.queryNames(gcName, null);
            for (final Object oname : names) {
                final ObjectName name = (ObjectName) oname;
                final GarbageCollectorMXBean gc = ManagementFactory.newPlatformMXBeanProxy(conn, name.getCanonicalName(), GarbageCollectorMXBean.class);
                System.out.println("Garbage name:" + gc.getName());
                System.out.println("Garbage Collection Count:" + gc.getCollectionCount());
                System.out.println("Garbage Collection Time:" + gc.getCollectionTime() + "ms");
            }
            System.out.println();
        } catch (final Exception e) {
            e.printStackTrace();
        }
    }
}
