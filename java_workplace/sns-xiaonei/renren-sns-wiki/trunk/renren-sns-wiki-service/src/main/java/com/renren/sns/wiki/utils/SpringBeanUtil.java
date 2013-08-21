package com.renren.sns.wiki.utils;

import net.paoding.rose.scanning.context.RoseAppContext;

/**
 * Spring 工具类
 * 
 * @author yi.li@renren-inc.com since 2012-5-7
 * 
 */
public class SpringBeanUtil {

    private static volatile SpringBeanUtil instance = null;

    private static Object lock = new Object();

    private RoseAppContext ctx;

    private SpringBeanUtil() {
        ctx = new RoseAppContext("com.renren.sns.wiki, com.xiaonei.jade, net.paoding.rose.jade",
                true);
    }

    public static SpringBeanUtil getInstance() {
        if (instance == null) {
            synchronized (lock) {
                if (instance == null) {
                    instance = new SpringBeanUtil();
                }
            }
        }

        return instance;
    }

    public <T> T getBean(Class<T> clazz) {
        return instance.ctx.getBean(clazz);
    }

}
