package com.renren.commons.tools.util;

import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;

public class JVMMemoryUtil {

    /**
     * @Author xuze(cantellow)
     * @Time Jan 10, 2012 10:57:57 AM
     * @return 返回是byte
     * @Todo 获取当前内存分配情况
     * @Modify
     */
    public static long getUsedMemory() {
        final MemoryMXBean memorymbean = ManagementFactory.getMemoryMXBean();
        final MemoryUsage usage = memorymbean.getHeapMemoryUsage();
        return usage.getUsed();
    }
}
