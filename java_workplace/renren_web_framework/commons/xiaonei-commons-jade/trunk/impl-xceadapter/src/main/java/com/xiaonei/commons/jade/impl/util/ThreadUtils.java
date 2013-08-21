package com.xiaonei.commons.jade.impl.util;

import com.meidusa.amoeba.parser.dbobject.Column;

/**
 * 实现公共线程局部变量的访问。
 * 
 * @author han.liao
 */
public class ThreadUtils {

    protected static class ThreadContext {

        protected boolean forceWrite = false;

        protected Column shardColumn = null;

        protected Object shardValue = null;
    }

    protected static ThreadLocal<ThreadContext> contextThreadLocal = new ThreadLocal<ThreadContext>();

    /**
     * 检查数据源是否强制访问写。
     * 
     * @return 数据源是否强制访问写
     */
    public static boolean isForceWrite() {

        ThreadContext context = contextThreadLocal.get();

        if (context != null) {
            return context.forceWrite;
        }

        return false;
    }

    /**
     * 设置数据源是否强制访问写。
     * 
     * @param forceWrite - 数据源是否强制访问写
     */
    public static void setForceWrite(boolean forceWrite) {

        ThreadContext context = createContext();
        context.forceWrite = forceWrite;
    }

    /**
     * 获取外部传入的散表列。
     * 
     * @return 外部传入的散表列
     */
    public static Column getShardColumn() {

        ThreadContext context = contextThreadLocal.get();
        if (context != null) {
            return context.shardColumn;
        }

        return null;
    }

    /**
     * 设置外部传入的散表列。
     * 
     * @return 设置传入的散表列
     */
    public static void setShardColumn(Column column) {

        ThreadContext context = createContext();
        context.shardColumn = column;
    }

    /**
     * 获取外部传入的散表参数。
     * 
     * @return 外部传入的散表参数
     */
    public static Object getShardParam() {

        ThreadContext context = contextThreadLocal.get();
        if (context != null) {
            return context.shardValue;
        }

        return null;
    }

    /**
     * 设置外部传入的散表参数。
     * 
     * @param shardValue - 外部传入的散表参数
     */
    public static void setShardParam(Object shardValue) {

        ThreadContext context = createContext();
        context.shardValue = shardValue;
    }

    /**
     * 清除设置的所有线程上下文变量。
     */
    public static void clearAll() {

        contextThreadLocal.remove();
    }

    // 创建上下文信息
    protected static ThreadContext createContext() {

        ThreadContext context = contextThreadLocal.get();

        if (context == null) {
            context = new ThreadContext();
            contextThreadLocal.set(context);
        }

        return context;
    }
}
