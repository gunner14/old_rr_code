package com.renren.xoa2.client;

import java.lang.reflect.Proxy;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.client.util.ClassUtils;
import com.renren.xoa2.exception.XoaClassNotFoundException;
import com.renren.xoa2.exception.XoaNoSuchMethodException;

public class XmqServiceFactory implements IServiceFactory {
    private ConcurrentMap<Class<?>, Object> serviceCache = new ConcurrentHashMap<Class<?>, Object>();

    @Override
    public <T> T getService(Class<T> serviceInterface) {
        return getService(serviceInterface, 250);
    }

    @Override
    public <T> T getService(Class<T> serviceClass, long timeout) {
        Object serviceInstance = serviceCache.get(serviceClass);

        if (serviceInstance != null) { // 先从缓存中找类定义，缓存中有数据直接返回
            return (T) serviceInstance;
        }

        try {            
            ClassDefinition serviceDefinition = new ClassDefinition(serviceClass);
            
            XmqServiceInvocationHandler handler = new XmqServiceInvocationHandler(serviceDefinition);
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
