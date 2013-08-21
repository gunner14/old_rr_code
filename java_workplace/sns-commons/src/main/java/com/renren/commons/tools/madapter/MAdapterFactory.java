package com.renren.commons.tools.madapter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.sf.cglib.proxy.Enhancer;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Mar 6, 2012 2:00:57 PM
 * @Who 得到适配器的工厂
 * @Modify
 */
public class MAdapterFactory {

    private static final Log mAdapterLogger = LogFactory.getLog("org.perf4j.MAdapterTimingLogger");

    @SuppressWarnings("unchecked")
    public static <T> T getSyncMAdapter(final T targetObject, final Object value, final int failsThreshold, final int timeOut, Log perfLogger,
            final InvocationFailedListener listener) {
        if (targetObject == null) {
            throw new NullPointerException("targetObject is null");
        }
        if (perfLogger == null) {
            perfLogger = mAdapterLogger;
        }
        //回调处理器
        final MSyncAdapterHandler handler = new MSyncAdapterHandler(targetObject, failsThreshold, timeOut, value, perfLogger, listener);
        final Enhancer enhancer = new Enhancer();
        //设置代理目标
        enhancer.setSuperclass(targetObject.getClass());
        //设置回调
        enhancer.setCallback(handler);
        //显示设置使用缓存，不会每次都新建一个类
        enhancer.setUseCache(true);
        enhancer.setClassLoader(targetObject.getClass().getClassLoader());
        final Object proxyObject = enhancer.create();
        return (T) proxyObject;
    }

    /**
     * @Author xuze(cantellow)
     * @Time Apr 6, 2012 7:02:02 PM
     * @param <T>
     * @param targetObject 目标对象
     * @param value 设定好的返回值
     * @param perfLogger 
     *        用于perf4j的log，如果为null，需要仿照perf4j默认配置org.perf4j.MAdapterTimingLogger
     * @param failedListener 失败的监听器，方便调用者失败通知，为null则直接返回默认值
     * @return
     * @Todo 得到同步调用对象，这个对象是包装器的子类
     * @Modify
     */
    public static <T> T getSyncMAdapter(final T targetObject, final Object value, final Log perfLogger, final InvocationFailedListener failedListener) {
        return getSyncMAdapter(targetObject, value, 5, 3, perfLogger, failedListener);
    }
}
