package com.renren.xoa2.client;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TTransportException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.thriftex.transport.XMQTransport;
import com.renren.xoa2.client.definition.ClassDefinition;
import com.renren.xoa2.client.definition.MethodDefinition;
import com.renren.xoa2.exception.XoaClientPoolException;
import com.renren.xoa2.exception.XoaTransportException;

public class XmqServiceInvocationHandler implements InvocationHandler {
    private ClassDefinition serviceDefinition = null;

    private ConcurrentMap<Method, MethodDefinition> methodCache = new ConcurrentHashMap<Method, MethodDefinition>();

    private static Logger logger = LoggerFactory.getLogger(XmqServiceInvocationHandler.class);

    public XmqServiceInvocationHandler(ClassDefinition serviceDefinition) {
        if (serviceDefinition == null) {
            throw new NullPointerException();
        }

        this.serviceDefinition = serviceDefinition;
    }
    
    private XMQTransport producerTrans = null; 
    private XMQTransport getProducerTransport() {
        if (producerTrans == null) {
            // TODO : transition on reg change
            producerTrans = new XMQTransport("tcp://10.11.21.185:56304",
                    XMQTransport.ProducerTrans);
            try {
                producerTrans.open();
            } catch (TTransportException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        return producerTrans;
    }
    
    @Override
    public Object invoke(Object proxy, Method method, Object[] args)
            throws Throwable {

        MethodDefinition methodDef = null;
        try {
            methodDef = getRealMethod(method);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } // 可能会抛出异常

        try {
            TProtocol protocol = new TCompactProtocol(getProducerTransport());
            Object client = serviceDefinition.getServiceClientConstructor()
                    .newInstance(protocol);

            Object ret = methodDef.getMethod().invoke(client, args);
        } catch (InvocationTargetException e) {
            Throwable cause = e.getCause();
            if (cause instanceof org.apache.thrift.TBase) {
                logger.error("service returned ex");
            } else {
            }
            throw cause;
        } catch (Exception e) {
            Throwable cause = e.getCause();
            if (cause == null) {
                throw e;
            } else {
                throw cause;
            }
        }

        return null;
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
