package com.renren.commons.tools.findunusedjar;

import java.io.File;
import java.io.IOException;
import java.lang.management.ClassLoadingMXBean;
import java.lang.management.ManagementFactory;

import javax.management.MBeanServerConnection;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;

import com.sun.tools.attach.VirtualMachine;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Apr 15, 2012 1:50:12 PM
 * @Who 打印类加载信息
 * @Modify
 */
@SuppressWarnings("restriction")
public class ClassLoadingUtil {

    private static final String connectorAddressProperty = "com.sun.management.jmxremote.localConnectorAddress";

    public static void main(final String[] args) {
        if (args.length != 2) {
            throw new IllegalArgumentException("argument is wrong..");
        }
        final long pid = Long.parseLong(args[0]);
        final ClassLoadingUtil util = new ClassLoadingUtil();
        util.getUnloadedClassCount(pid);
    }

    public long getUnloadedClassCount(final long pid) {
        MBeanServerConnection serviceConnection = null;
        VirtualMachine vm = null;
        try {
            vm = VirtualMachine.attach(String.valueOf(pid));
            String connectorAddress = vm.getAgentProperties().getProperty(connectorAddressProperty);
            if (connectorAddress == null) {
                final String agent = vm.getSystemProperties().getProperty("java.home") + File.separator + "lib" + File.separator + "management-agent.jar";
                vm.loadAgent(agent);
                connectorAddress = vm.getAgentProperties().getProperty(connectorAddressProperty);
            }
            if (connectorAddress == null) {
                System.exit(1);
            }
            serviceConnection = this.getMBeanServerConnection(connectorAddress);
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return this.getUnloadedClassCount0(serviceConnection);
    }

    private MBeanServerConnection getMBeanServerConnection(final String connectorAddress) throws IOException {
        final JMXServiceURL url = new JMXServiceURL(connectorAddress);
        final JMXConnector connector = JMXConnectorFactory.connect(url);
        final MBeanServerConnection remote = connector.getMBeanServerConnection();
        return remote;
    }

    private long getUnloadedClassCount0(final MBeanServerConnection conn) {
        ClassLoadingMXBean clMBean = null;
        try {
            clMBean = ManagementFactory.newPlatformMXBeanProxy(conn, ManagementFactory.CLASS_LOADING_MXBEAN_NAME, ClassLoadingMXBean.class);
        } catch (final IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println();
        System.out.println("TotalLoadedClassCount:" + clMBean.getTotalLoadedClassCount());
        System.out.println("LoadedClassCount:" + clMBean.getLoadedClassCount());
        System.out.println("UnloadedClassCount:" + clMBean.getUnloadedClassCount());
        //here we can turn on verbose of class loading dynamically
        //clMBean.setVerbose(verbose);
        return clMBean.getUnloadedClassCount();
    }
}
