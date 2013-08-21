package com.renren.sns.wiki.utils.event.impl.thread;

/**
 * 调用路径的ThreadLocal工具类
 * 
 * @author yi.li@renren-inc.com since 2012-3-31
 * 
 */
public class InvocationPathUtil {

    private static ThreadLocal<String> invocationPathLocal = new ThreadLocal<String>();

    /** 调用路径分割符 */
    public static final String INV_PATH_SEPERATOR = ".";

    /** 调用路径分隔符（正则） */
    public static final String REG_INV_PATH_SEPERATOR = "\\.";

    /**
     * 绑定{@code invocationPath}到当前线程
     * 
     * @param invocationPath
     */
    public static void bindInvocationToCurrentPath(String invocationPath) {
        invocationPathLocal.set(invocationPath);
    }

    /**
     * 获取当前线程的调用路径
     */
    public static String getCurrentInvocationPath() {
        String invocationPath = invocationPathLocal.get();

        if (null == invocationPath) {
            return INV_PATH_SEPERATOR;
        }
        return invocationPath;
    }

    /**
     * 清除当前线程的调用路径数据
     */
    public static void unbindCurrentInvocationPath() {
        invocationPathLocal.remove();
    }
}
