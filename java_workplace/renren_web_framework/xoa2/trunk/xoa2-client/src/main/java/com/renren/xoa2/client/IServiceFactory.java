package com.renren.xoa2.client;
/**
 * 
 * @author xun.dai@renren-inc.com
 * 抽象IServiceFactory。
 *
 */
public interface IServiceFactory {

    public <T> T getService(Class<T> serviceInterface);

    public <T> T getService(Class<T> serviceInterface, long timeout);

}
