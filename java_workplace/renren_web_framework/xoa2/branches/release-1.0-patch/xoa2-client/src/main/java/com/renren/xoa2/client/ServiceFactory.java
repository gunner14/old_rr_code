package com.renren.xoa2.client;


/**
 * 一个静态工厂类（工具类），根据接口生成代理服务类。
 * 使用IServiceFactory的默认实现创建代理服务类。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @author yanghe.liang
 * @since Nov 29, 2011 6:06:49 PM
 */
public class ServiceFactory {

    /**
     * 真正的工厂类,抽象IServiceFactory，目的在于封装创建Service实例的逻辑，与静态工厂类ServiceFactory解耦。
     * 
     * by xun.dai@renren-inc.com
     * 
     */
    private static IServiceFactory factory = new DefaultServiceFactory(CommonServiceRouter.getInstance());

    public static <T> T getService(Class<T> serviceClass) {
        return getService(serviceClass, 250);
    }

    public static <T> T getService(Class<T> serviceClass, int timeout) {
        return factory.getService(serviceClass, timeout);
    }
}
