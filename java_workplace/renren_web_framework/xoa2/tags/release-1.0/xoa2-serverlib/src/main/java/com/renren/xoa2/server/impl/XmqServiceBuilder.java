package com.renren.xoa2.server.impl;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.server.TServer;
import org.apache.thrift.transport.TServerTransport;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.thriftex.server.XMQSimpleServer;
import com.renren.thriftex.server.XMQSimpleServer.XMQArgs;
import com.renren.xoa2.registry.impl.zookeeper.XmqEndpointType;
import com.renren.xoa2.registry.impl.zookeeper.XmqRegistry;
import com.renren.xoa2.server.IInitListener;
import com.renren.xoa2.server.processor.XoaProcessor;

public class XmqServiceBuilder extends AbstractServiceBuilder {
    private static Logger logger = LoggerFactory.getLogger(XmqServiceBuilder.class);
    
    // Initialization on Demand Holder idiom
    public static XmqRegistry getXmqRegistry() {
        return XmqRegistryHolder.instance;
    }

    static class XmqRegistryHolder {
        static XmqRegistry instance = new XmqRegistry();
    }
    
    @Override
    protected TProcessor buildProcessor(XoaServiceConfig config) {
        // processor 是完全一样的
        final String className = config.getServiceClass();

        if (!className.endsWith("Impl")) {
            throw new IllegalArgumentException("Service class name must be end with 'Impl': "
                    + className);
        }
        String interfaceClassName = className.substring(0, className.length() - "Impl".length());

        try {
            Class<?> ifaceClass = Class.forName(interfaceClassName + "$Iface");
            Class<?> processorClass = Class.forName(interfaceClassName + "$Processor");
            
            Constructor<?> constructor = processorClass.getConstructor(ifaceClass);
            Object impl = Class.forName(className).newInstance();
            TProcessor processor = (TProcessor) constructor.newInstance(impl);

            return new XoaProcessor(processor);
        } catch (ClassNotFoundException e) {
            logger.error("", e);
        } catch (SecurityException e) {
            logger.error("", e);
        } catch (NoSuchMethodException e) {
            logger.error("", e);
        } catch (InstantiationException e) {
            logger.error("", e);
        } catch (IllegalAccessException e) {
            logger.error("", e);
        } catch (IllegalArgumentException e) {
            logger.error("", e);
        } catch (InvocationTargetException e) {
            logger.error("", e);
        }
        return null;
    }

    @Override
    protected TServerTransport buildTServerTransport(XoaServiceConfig config) {
        // 不需要server transport, 只要zmq connect到writer即可
        return null;
    }


    @Override
    protected TServer buildTServer(XoaServiceConfig config,
            TServerTransport serverTransport, TProcessor processor) {        
        XMQArgs args = new XMQArgs();
        args.processor(processor); // 指定processor

        String endpoint = getXmqRegistry().getEndpoint(config.getServiceId(), 
                XmqEndpointType.WRITER);
        if (endpoint == null) {
            return null;
        }
        
        System.out.println("writer endpoint : " + endpoint);
        // 没有监听端口，无需指定server transport
        args.endpoint("tcp://" + endpoint);
        args.protocolFactory(new TCompactProtocol.Factory());

        return new XMQSimpleServer(args);
    }

    @Override
    protected IInitListener buildInitListener(XoaServiceConfig config) {
        // TODO Auto-generated method stub
        return null;
    }

}
