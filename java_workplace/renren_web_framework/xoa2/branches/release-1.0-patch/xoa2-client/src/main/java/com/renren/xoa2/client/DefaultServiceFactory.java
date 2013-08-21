package com.renren.xoa2.client;

import java.lang.reflect.Proxy;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.client.util.ClassUtils;
import com.renren.xoa2.exception.XoaClassNotFoundException;
import com.renren.xoa2.exception.XoaNoSuchMethodException;


/**
 * 封装创建Service实例的逻辑，缓存Service的Proxy实例，IServiceFactory的默认实现。
 * @author xun.dai@renren-inc.com 
 *
 */
public class DefaultServiceFactory implements IServiceFactory {

    private ConcurrentMap<Class<?>, Object> serviceCache = new ConcurrentHashMap<Class<?>, Object>();

    private ServiceRouter serviceRouter;
    
    
    public DefaultServiceFactory(ServiceRouter serviceRouter) {
        if(serviceRouter == null){
            throw new NullPointerException();
        }
        this.serviceRouter = serviceRouter;
    }

    @Override
    public <T> T getService(Class<T> serviceInterface) {
        return getService(serviceInterface, 250);
    }
    
    @SuppressWarnings("unchecked")
    public <T> T getService(Class<T> serviceClass, long timeout) {
        
        Object serviceInstance = serviceCache.get(serviceClass);

        if (serviceInstance != null) { // 先从缓存中找类定义，缓存中有数据直接返回
            return (T) serviceInstance;
        }

        try {
            
            ClassDefinition serviceDefinition = new ClassDefinition(serviceClass);
            serviceRouter.setTimeout(timeout);
            ServiceInvocationHandler handler = new ServiceInvocationHandler(serviceRouter, serviceDefinition);
            T proxy = (T) Proxy.newProxyInstance(ClassUtils.getDefaultClassLoader(),
                    new Class<?>[] { serviceClass }, handler);

            Object tmp = serviceCache.putIfAbsent(serviceClass, proxy);
            if (tmp != null) return (T) tmp; // 保证返回单例

            return proxy;
        } catch (ClassNotFoundException e) {
            throw new XoaClassNotFoundException(e);
        } catch (NoSuchMethodException e) {
            throw new XoaNoSuchMethodException(e);
        }
    }
    
}
