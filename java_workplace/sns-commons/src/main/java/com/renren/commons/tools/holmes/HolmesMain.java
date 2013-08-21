package com.renren.commons.tools.holmes;

import java.io.File;
import java.io.IOException;

import javax.management.MBeanServerConnection;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;

import com.renren.commons.tools.holmes.mxbean.ClassLoadingInfo;
import com.renren.commons.tools.holmes.mxbean.CompilationInfo;
import com.renren.commons.tools.holmes.mxbean.GarbageCollectorInfo;
import com.renren.commons.tools.holmes.mxbean.OperatingSystemInfo;
import com.renren.commons.tools.holmes.mxbean.RuntimeInfo;
import com.renren.commons.tools.holmes.mxbean.ThreadInfo;
import com.sun.tools.attach.VirtualMachine;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Apr 21, 2012 6:32:48 PM
 * @Who 福尔摩斯，利用JVM
 *      SA特性去查看一个java进程的一些特性，方便排查问题，其实也可以做成后台页面，但Kitty已经实现了这样的功能。
 * @Modify
 */
public class HolmesMain {

    private static final String connectorAddressProperty = "com.sun.management.jmxremote.localConnectorAddress";

    public static void main(final String[] args) {
        if (args.length != 1) {
            throw new IllegalArgumentException("no java pid..");
        }
        final long pid = Long.parseLong(args[0]);
        final HolmesMain hm = new HolmesMain();
        final MBeanServerConnection msc = hm.getMBeanServerConnection(pid);
        //操作系统信息
        OperatingSystemInfo.printOperatingSystemInfo(msc);
        //JVM信息
        RuntimeInfo.printRuntimeInfo(msc);
        //类加载信息
        ClassLoadingInfo.printClassLoadInfo(msc);
        //JIT编译信息
        CompilationInfo.printCompilationInfo(msc);
        //垃圾回收信息
        GarbageCollectorInfo.printGarbageCollectorInfo(msc);
        //线程信息
        ThreadInfo.printThreadInfo(msc);
        //NIO信息
        DirectMemorySize.main(args);
    }

    private MBeanServerConnection getMBeanServerConnection(final long pid) {
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
                System.err.println("something error..");
                System.exit(1);
            }
            serviceConnection = this.getMBeanServerConnection0(connectorAddress);
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return serviceConnection;
    }

    private MBeanServerConnection getMBeanServerConnection0(final String connectorAddress) throws IOException {
        final JMXServiceURL url = new JMXServiceURL(connectorAddress);
        final JMXConnector connector = JMXConnectorFactory.connect(url);
        final MBeanServerConnection remote = connector.getMBeanServerConnection();
        return remote;
    }
}
