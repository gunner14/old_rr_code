package com.renren.xoa2.client;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.client.definition.MethodDefinition;
import com.renren.xoa2.client.transport.XoaTransport;
import com.renren.xoa2.exception.XoaClientPoolException;
import com.renren.xoa2.exception.XoaTransportException;

/**
 * proxy的InvocationHandler
 * 
 * @author yanghe.liang
 * @since 2011-12-11 上午11:18:49
 */
public class ServiceInvocationHandler implements InvocationHandler {

    private static Logger logger = LoggerFactory.getLogger(ServiceInvocationHandler.class);
    private ClassDefinition serviceDefinition;

    private ConcurrentMap<Method, MethodDefinition> methodCache = new ConcurrentHashMap<Method, MethodDefinition>();

    private ServiceRouter serviceRouter;

    public ServiceInvocationHandler(ClassDefinition serviceDefinition) {
        this(CommonServiceRouter.getInstance(), serviceDefinition);
    }

    public ServiceInvocationHandler(ServiceRouter serviceRouter, ClassDefinition serviceDefinition) {
        if (serviceDefinition == null || serviceRouter == null) {
            throw new NullPointerException();
        }
        this.serviceRouter = serviceRouter;
        this.serviceDefinition = serviceDefinition;
    }

    private static int MAX_RETRY = 1;
    
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

        MethodDefinition methodDef = null;
        try {
            methodDef = getRealMethod(method);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } // 可能会抛出异常
        
        String serviceId = serviceDefinition.getServiceId();
        XoaTransport xoaTransport = null;
        int retry = 0;
        try {
            while (true) {
                xoaTransport = serviceRouter.routeService(serviceId, null);

                if (xoaTransport != null) {
                    break;
                }

                if (++retry >= MAX_RETRY) {
                    logger.error(serviceId + " get xoaTransport failed.");
                    return null;
                }
            }
            TProtocol protocol = new TBinaryProtocol(xoaTransport.getTransport());
            Object client = serviceDefinition.getServiceClientConstructor().newInstance(protocol);
            
            Object ret = methodDef.getMethod().invoke(client, args);
            
            serviceRouter.returnConn(xoaTransport);
            return ret;            
        } catch (XoaClientPoolException e) {
            serviceRouter.serviceException(serviceId, e, xoaTransport);
            throw e;
        } catch (XoaTransportException e) {
            serviceRouter.serviceException(serviceId, e, xoaTransport);
            throw e;
        } catch (InvocationTargetException e) {
            // TODO : 如何准确判断此种情况？
            Throwable cause = e.getCause();
            if (cause instanceof org.apache.thrift.TBase) {
                serviceRouter.returnConn(xoaTransport);
                logger.error("service returned ex");
            } else {
                serviceRouter.serviceException(serviceId, cause, xoaTransport);
            }
            throw cause;
        } catch (Exception e) {
            Throwable cause = e.getCause();
            if (cause == null) {
                serviceRouter.serviceException(serviceId, e, xoaTransport);
                throw e;
            } else {
                serviceRouter.serviceException(serviceId, cause, xoaTransport);
                throw cause;
            }
        }
    }

    /**
     * 获得方法定义，首先会从缓存中取数据，如果缓存中没有则通过反射的方式获得
     * 
     * @param method
     * @return
     * @throws SecurityException
     * @throws NoSuchMethodException
     */
    private MethodDefinition getRealMethod(Method method) throws SecurityException,
            NoSuchMethodException {
        MethodDefinition methodDef = methodCache.get(method);

        if (methodDef != null) { // 先从缓存中找方法定义，缓存中有数据直接返回
            return methodDef;
        }

        Method realMethod = serviceDefinition.getServiceClientClass().getMethod(method.getName(),
                method.getParameterTypes());
        methodDef = new MethodDefinition(realMethod);
        methodCache.put(method, methodDef);
        return methodDef;
    }
}
