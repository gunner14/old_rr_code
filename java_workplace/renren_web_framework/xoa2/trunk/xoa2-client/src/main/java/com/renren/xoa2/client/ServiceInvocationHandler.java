package com.renren.xoa2.client;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.client.definition.MethodDefinition;
import com.renren.xoa2.client.transport.XoaTransport;
import com.renren.xoa2.exception.XoaTransportException;

/**
 * proxy的InvocationHandler
 * 
 * @author yanghe.liang
 * @since 2011-12-11 上午11:18:49
 */
public class ServiceInvocationHandler implements InvocationHandler {

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

    private static int MAX_RETRY = 3;

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

        XoaTransport xoaTransport = null;
        int retry = 0;
        try {
            while (true) {
                xoaTransport = serviceRouter.routeService(serviceDefinition.getServiceId(), null);

                if (xoaTransport != null && !xoaTransport.isDisabled()) {
                    break;
                }

                if (xoaTransport != null) {
                    // 不可用的 node
                    Exception e = new Exception("Bad service :" + xoaTransport.getNode().getHost()
                            + xoaTransport.getNode().getPort());
                    serviceRouter.serviceException(e, xoaTransport);
                }
                if (++retry > MAX_RETRY) {
                    throw new XoaTransportException("get xoaTransport failure with " + MAX_RETRY
                            + " retry");
                }
            }

            TProtocol protocol = new TBinaryProtocol(xoaTransport.getTransport());
            Object client = serviceDefinition.getServiceClientConstructor().newInstance(protocol);

            Object ret = methodDef.getMethod().invoke(client, args);

            serviceRouter.returnConn(xoaTransport);
            return ret;
        } catch (Exception e) {
            serviceRouter.serviceException(e, xoaTransport); // 让连接池处理异常
            throw e;
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
